
#ifndef _ACTOREXPORTER_H_
#define _ACTOREXPORTER_H_

#include "IExporter.h"
#include "TGeoBuilder.h"
#include "../Game/ActorFileFormat.h"
#include <IGame/IGameModifier.h>

typedef Core::TGeoBuilder<Core::Vx3D>		StdBuilder;
typedef Core::TGeoBuilder<Core::Vx3DSkin>	SkinBuilder;

class ActorSub
{
public:

	int					triCount;
	int					triOffset;
	char				name[16];

	ActorSub()			{ name[0]='\0'; }
	ActorSub(const ActorSub& as) : triCount(as.triCount), triOffset(as.triOffset)
	{
		strcpy_s(name,as.name);
	}

	ActorSub&			operator=(const ActorSub& s)
	{
		triCount=s.triCount;
		triOffset=s.triOffset;
		strcpy_s(name,s.name);
		return *this;
	}
};

class ActorNullNode
{
public:

	int					id;
	int					parent;
	char				name[16];
	IGameNode*			node;

	ActorNullNode()	{ name[0]='\0'; }
	ActorNullNode(const ActorNullNode& n) : id(n.id), node(n.node), parent(n.parent)
	{
		strcpy_s(name,n.name);
	}

	ActorNullNode& operator=(const ActorNullNode& n)
	{
		id=n.id;
		parent=n.parent;
		strcpy_s(name,n.name);
		node=n.node;
		return *this;
	}
};

class ActorExporter : public IExporter
{
protected:

	File::ActorFileHeader				_header;
	Core::TDynamicTab<ActorSub>			_subs;
	Core::TDynamicTab<ActorNullNode>	_nullNodes;
	Core::TDynamicTab<Core::Matrix4>	_pose;
	Core::TDynamicTab<int>				_parents;
	Core::TDynamicTab<Core::Matrix4>	_boneMatrices;
	Core::TDynamicTab<IGameNode*>		_boneNodes;

	bool				isSkinned() const;
	void				exportStdNode(IGameNode* n, int parent, StdBuilder& builder);
	void				exportSkinned(SkinBuilder& builder);
	void				exportBone(IGameNode* n, int parent);
	void				exportSkinnedMesh(IGameNode* n, IGameMesh* m, SkinBuilder& builder);
	void				exportBoneMatrices(IGameSkin *s);
	void				processNullNodes();

	void				addPoseMatrix(IGameNode *n, int parent);

	void				initBoneInfos(Core::Vx3DSkin& v) const
	{
		v.dwBoneIndices=0;
		for(int i=0;i<4;++i)
			v.boneWeights[i]=0;
	}
	void				normalizeBoneInfos(Core::Vx3DSkin& v) const
	{
		float total=0;
		for(int i=0;i<3;++i)			total+=v.boneWeights[i];

		total=total==0 ? 0 : 1.0f / total;
		for(int i=0;i<3;++i)			v.boneWeights[i]*=total;
	}
	unsigned char		getBoneIndex(IGameNode *n) const
	{
		for(int i=0;i<_boneNodes.getCount();++i)
			if(n==_boneNodes[i])
				return i;

		return 0;
	}

public:

	ActorExporter();
	~ActorExporter();

	void				doExport(const string& name);
};

#endif