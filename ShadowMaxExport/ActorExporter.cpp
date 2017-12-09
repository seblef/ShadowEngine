
#include "stdafx.h"
#include "ActorExporter.h"

ActorExporter::ActorExporter()
{
}

ActorExporter::~ActorExporter()
{
}

void ActorExporter::doExport(const string& name)
{
	_header._box.setMin(Core::Vector3(1e23f,1e23f,1e23f));
	_header._box.setMax(Core::Vector3(-1e23f,-1e23f,-1e23f));

	bool skin;
	Core::GeoBuilder *builder=0;

	if(isSkinned())
	{
		builder=new SkinBuilder;
		exportSkinned(*((SkinBuilder*)builder));
		skin=true;
	}
	else
	{
		skin=false;
		builder=new StdBuilder;
		exportStdNode(_scene->GetTopLevelNode(0),-1,*((StdBuilder*)builder));
	}

	processNullNodes();

	ofstream ofs(name,ios::out | ios::binary);
	if(ofs.good())
	{
		_header._header=ACTOR_HEADER;
		_header._vxCount=builder->getVerticesCount();
		_header._triCount=builder->getIndices().getCount() / 3;
		_header._box=builder->getBBox();
		_header._subCount=_subs.getCount();
		_header._boneCount=_boneMatrices.getCount();
		_header._nullNodeCount=_nullNodes.getCount();

		ofs.write((const char*)&_header,sizeof(File::ActorFileHeader));
		ofs.write((const char*)builder->getVerticesBuffer(),builder->getVertexSize() * _header._vxCount);
		ofs.write((const char*)builder->getIndices().getBuffer(),sizeof(unsigned short) * 3 * _header._triCount);

		File::ActorFileNullNode nn;
		for(int i=0;i<_nullNodes.getCount();++i)
		{
			nn._id=_nullNodes[i].id;
			strcpy_s(nn._name,_nullNodes[i].name);
			ofs.write((const char*)&nn,sizeof(File::ActorFileNullNode));
		}

		if(skin)
			ofs.write((const char*)_boneMatrices.getBuffer(),sizeof(Core::Matrix4) * _boneMatrices.getCount());
		else
		{
			File::ActorFileSub as;
			for(int i=0;i<_subs.getCount();++i)
			{
				as._triCount=_subs[i].triCount;
				as._triOffset=_subs[i].triOffset;
				strcpy_s(as._name,_subs[i].name);
				ofs.write((const char*)&as,sizeof(File::ActorFileSub));
			}
		}

		ofs.write((const char*)_pose.getBuffer(),sizeof(Core::Matrix4)*_pose.getCount());
		ofs.write((const char*)_parents.getBuffer(),sizeof(int)*_parents.getCount());

		assert(_pose.getCount()==_parents.getCount());
	}

	if(builder)		delete builder;
}

bool ActorExporter::isSkinned() const
{
	return _scene->GetIGameNodeByType(IGameObject::IGAME_BONE).Count() > 0;
}

void ActorExporter::exportStdNode(IGameNode *node, int parent, StdBuilder& builder)
{
	string nodeName(node->GetName());
	if(nodeName.length() > 4 && nodeName.substr(0,4)=="null")
	{
		ActorNullNode nn;
		nn.parent=parent;
		nn.node=node;
		_nullNodes.add(nn);
		return;
	}

	IGameObject *o=node->GetIGameObject();
	if(o->GetIGameType()!=IGameObject::IGAME_MESH)
	{
		node->ReleaseIGameObject();
		return;
	}

	IGameMesh *m=(IGameMesh*)o;
	int offset=builder.getIndices().getCount() / 3;
	m->InitializeData();

	builder.processMesh(node,m,true);

	ActorSub as;
	as.triCount=m->GetNumberOfFaces();
	as.triOffset=offset;
	strcpy_s(as.name,node->GetName());

	_subs.add(as);
	addPoseMatrix(node,parent);

	node->ReleaseIGameObject();

	int this_id=_pose.getCount()-1;
	for(int i=0;i<node->GetChildCount();++i)
		exportStdNode(node->GetNodeChild(i),this_id,builder);
}

void ActorExporter::exportSkinned(SkinBuilder& builder)
{
	Tab<IGameNode*> bones(_scene->GetIGameNodeByType(IGameObject::IGAME_BONE));
	IGameNode *b=bones[0];
	while(b->GetNodeParent())
		b=b->GetNodeParent();

	exportBone(b,-1);

	Tab<IGameNode*> meshes(_scene->GetIGameNodeByType(IGameObject::IGAME_MESH));
	IGameObject *o;
	for(int i=0;i<meshes.Count();++i)
	{
		o=meshes[i]->GetIGameObject();
		if(o->IsObjectSkinned())
		{
			exportSkinnedMesh(meshes[i],(IGameMesh*)o,builder);
			break;
		}
	}
}

void ActorExporter::exportBone(IGameNode *n, int parent)
{
	string nodeName(n->GetName());
	if(nodeName.length() > 4 && nodeName.substr(0,4)=="null")
	{
		ActorNullNode nn;
		nn.parent=parent;
		nn.node=n;
		_nullNodes.add(nn);
		return;
	}

	IGameObject *o=n->GetIGameObject();
	if(o->GetIGameType()!=IGameObject::IGAME_BONE)
	{
		n->ReleaseIGameObject();
		return;
	}

	Core::Matrix4 mlocal;
	mlocal.createIdentity();

	addPoseMatrix(n,parent);
	_boneNodes.add(n);
	_boneMatrices.add(mlocal);
	n->ReleaseIGameObject();

	int this_id=_pose.getCount()-1;
	strcpy_s(_header._bonesNames[this_id], nodeName.c_str());

	for(int i=0;i<n->GetChildCount();++i)
		exportBone(n->GetNodeChild(i),this_id);
}

void ActorExporter::exportSkinnedMesh(IGameNode *n, IGameMesh* m, SkinBuilder& builder)
{
	m->InitializeData();
	IGameSkin *s=m->GetIGameSkin();
	IGameMesh *o=s->GetInitialPose();
	o->InitializeData();
	builder.processMesh(n,o,true);

	Core::TDynamicTab<Core::Vx3DSkin>& verts(builder.getVertices());
	Core::TDynamicTab<unsigned short>& ind(builder.getIndices());

	unsigned short c=0;
	int b_count;
	FaceEx *f;
	for(int i=0;i<o->GetNumberOfFaces();++i)
	{
		f=o->GetFace(i);
		for(int j=0;j<3;++j,++c)
		{
			Core::Vx3DSkin& v(verts[ind[c]]);
			initBoneInfos(v);
			b_count=smin(s->GetNumberOfBones(f->vert[j]),3);
			
			for(int k=0;k<b_count;++k)
			{
				v.boneIndices[k]=getBoneIndex(s->GetIGameBone(f->vert[j],k));
				v.boneWeights[k]=s->GetWeight(f->vert[j],k);
			}

			normalizeBoneInfos(v);
		}
	}

	exportBoneMatrices(s);
}

void ActorExporter::exportBoneMatrices(IGameSkin *s)
{
	GMatrix mBone;
	Core::Matrix4 mOriginal,mInvOriginal;
	Core::Matrix4 mSkin;

	s->GetInitSkinTM(mBone);
	copyMatrix(mBone,mSkin);
	validMatrix(mSkin);
	mSkin.inverse(_header._invSkinMatrix);

	for(int i=0;i<_boneMatrices.getCount();++i)
	{
		s->GetInitBoneTM(_boneNodes[i],mBone);
		copyMatrix(mBone,mOriginal);
		mOriginal.inverse(_boneMatrices[i]);
	}
}

void ActorExporter::processNullNodes()
{
	ActorNullNode *nn;
	Core::Matrix4 mlocal;
	for(int i=0;i<_nullNodes.getCount();++i)
	{
		nn=&_nullNodes[i];
		nn->id=_pose.getCount();
		strcpy_s(nn->name,nn->node->GetName()+4);
		addPoseMatrix(nn->node,nn->parent);
	}
}

void ActorExporter::addPoseMatrix(IGameNode *n, int parent)
{
	_parents.add(parent);

	Core::Matrix4 mlocal;
	copyMatrix(n->GetLocalTM(0),mlocal);
	validMatrix(mlocal);
	_pose.add(mlocal);
}