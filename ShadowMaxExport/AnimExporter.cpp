
#include "stdafx.h"
#include "AnimExporter.h"
#include "SUtils.h"

#define SHADOW_FRAME_RATE			25.0f

AnimExporter::AnimExporter() : _track(0)
{
}

AnimExporter::~AnimExporter()
{
	if(_track)			delete _track;
}

void AnimExporter::doExport(const string& name)
{
	if(isSkinned())
	{
		Tab<IGameNode*> bones(_scene->GetIGameNodeByType(IGameObject::IGAME_BONE));
		IGameNode *b=bones[0];
		while(b->GetNodeParent())
			b=b->GetNodeParent();

		collectNodes(b, IGameObject::IGAME_BONE);
	}
	else
		collectNodes(_scene->GetTopLevelNode(0),IGameObject::IGAME_MESH);

	for(int i=0;i<_nullNodes.size();++i)
		_nodes.push_back(_nullNodes[i]);

	processPRS();
	save(name);
}

bool AnimExporter::isSkinned() const
{
	return _scene->GetIGameNodeByType(IGameObject::IGAME_BONE).Count() > 0;
}

void AnimExporter::collectNodes(IGameNode *n, IGameObject::ObjectTypes type)
{
	string nodeName(n->GetName());
	IGameObject *o=n->GetIGameObject();
	if(nodeName.length() > 4 && nodeName.substr(0,4)=="null")
		_nullNodes.push_back(n);
	else if(o->GetIGameType()==type)
	{
		_nodes.push_back(n);

		for(int i=0;i<n->GetChildCount();++i)
			collectNodes(n->GetNodeChild(i),type);
	}

	n->ReleaseIGameObject();
}

void AnimExporter::processPRS()
{
	float startTime=TicksToSec(_scene->GetSceneStartTime());
	float animTime=TicksToSec(_scene->GetSceneEndTime()) - startTime;
	int keyCount=(int)(animTime * SHADOW_FRAME_RATE);
	int trackCount=_nodes.size();

	_track=new Core::AnimPRSMultiTrack(keyCount,trackCount);
	Core::Matrix4 mlocal;
	GMatrix glocal;

	float time=startTime;
	TimeValue mtime;
	for(int i=0;i<keyCount;++i)
	{
		mtime=(TimeValue)(time * ((float)TIME_TICKSPERSEC));//SecToTicks(time+startTime);
		_track->setTime(i,time-startTime);

		for(int j=0;j<_nodes.size();++j)
		{
			copyMatrix(_nodes[j]->GetLocalTM(mtime),mlocal);
			validMatrix(mlocal);
			_track->getValue(i,j)=mlocal;
		}

		time+=1.0f / SHADOW_FRAME_RATE;
	}

	for(int i=1;i<_track->getKeyCount();++i)
		_track->getValue(i,0).setPosition(_track->getValue(0,0).getPosition());
}

void AnimExporter::save(const string& name)
{
	ofstream ofs(name,ios::out | ios::binary);
	if(ofs.good())
	{
		File::ActorAnimFileHeader h;
		h._header=ACTORANIM_HEADER;
		h._keyCount=_track->getKeyCount();
		h._trackCount=_track->getTrackCount();

		ofs.write((const char*)&h,sizeof(File::ActorAnimFileHeader));
		ofs.write((const char*)_track->getTimes(),sizeof(float) * h._keyCount);
		ofs.write((const char*)_track->getValues(),sizeof(Core::AnimPRSKeyVal) * h._keyCount * h._trackCount);
	}
}