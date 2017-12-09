
#include "stdafx.h"
#include "ActorACTLoader.h"
#include "ActorFileFormat.h"

extern ofstream g_MediaOut;

Actor* ActorACTLoader::loadActor(const string& actorFile, Material* mat, IVideoDevice* device)
{
	Actor* a=0;
	ActorCreate_t ac;
	Vx3D* vx=0;
	Vx3DSkin *vxs=0;
	unsigned short *ind;

	string file(_T("Actors/"));
	file+=actorFile;

	IFile *fl=FileSystemFactory::getFileSystem()->openFile(file,FA_READ);
	if(!fl)
		return 0;

	File::ActorFileHeader h;
	fl->read(&h,sizeof(File::ActorFileHeader));

	if(h._header!=ACTOR_HEADER)
	{
		delete fl;
		return 0;
	}

	ac.skinned=h._boneCount > 0;
	ac.vxCount=h._vxCount;
	ac.triCount=h._triCount;
	ac.box=h._box;
	ac.material=mat;
	ac.nullNodeCount=h._nullNodeCount;
	ac.subCount=ac.skinned ? h._boneCount : h._subCount;
	ac.invSkinMatrix=h._invSkinMatrix;
	memcpy(&ac.bonesNames, h._bonesNames, sizeof(ActorBonesNames_t));

	if(ac.skinned)
	{
		vxs=new Vx3DSkin[h._vxCount];
		fl->read(vxs,sizeof(Vx3DSkin) * h._vxCount);
		ac.vertices=vxs;
	}
	else
	{
		vx=new Vx3D[h._vxCount];
		fl->read(vx,sizeof(Vx3D) * h._vxCount);
		ac.vertices=vx;
	}

	ind=new unsigned short[h._triCount*3];
	fl->read(ind,sizeof(unsigned short) * 3 * h._triCount);
	ac.triangles=ind;

	if(h._nullNodeCount > 0)
	{
		File::ActorFileNullNode nn;
		ac.nullNodes=new ActorNullNode[h._nullNodeCount];
		for(int i=0;i<h._nullNodeCount;++i)
		{
			fl->read(&nn,sizeof(File::ActorFileNullNode));
			ac.nullNodes[i].setID(nn._id);
			ac.nullNodes[i].setName(nn._name);
		}
	}
	else
		ac.nullNodes=0;

	if(ac.skinned)
	{
		ac.boneMatrices=new MatrixTab(h._boneCount);
		ac.subs=0;
		fl->read(ac.boneMatrices->getItems(),sizeof(Matrix4) * h._boneCount);
	}
	else
	{
		File::ActorFileSub as;
		ac.boneMatrices=0;
		ac.subs=new ActorSub[h._subCount];
		for(int i=0;i<h._subCount;++i)
		{
			fl->read(&as,sizeof(File::ActorFileSub));
			ac.subs[i].setTriCount(as._triCount);
			ac.subs[i].setTriOffset(as._triOffset);
			ac.subs[i].setName(as._name);
		}
	}

	int mh_count=ac.subCount + ac.nullNodeCount;
	MatrixHierarchy mh(mh_count,true);
	fl->read(mh.getItems(),sizeof(Matrix4) * mh_count);
	fl->read(mh.getParents(),sizeof(int) * mh_count);
	ac.initialPose=&mh;

	a=new Actor(ac,device);

	if(vx)				delete[] vx;
	if(vxs)				delete[] vxs;
	delete[] ind;

	delete fl;
	return a;
}