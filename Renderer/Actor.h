
#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "ActorSub.h"
#include "ActorNullNode.h"
#include "Material.h"

#define MAX_ACTOR_BONES					64

struct ActorBonesNames_t
{
	char						bonesNames[MAX_ACTOR_BONES][16];
};

struct ActorCreate_t
{
	bool						skinned;
	int							vxCount;
	int							triCount;
	const void*					vertices;
	const unsigned short*		triangles;
	BBox3						box;
	Material*					material;
	int							nullNodeCount;
	ActorNullNode*				nullNodes;
	int							subCount;
	ActorSub*					subs;
	const MatrixHierarchy*		initialPose;
	MatrixTab*					boneMatrices;
	Matrix4						invSkinMatrix;
	ActorBonesNames_t			bonesNames;
};

class Actor
{
protected:

	typedef map<string,AnimPRSMultiTrack*>	AnimMap;

	bool				_skinned;

	Material*			_material;
	IIndexBuffer*		_iBuffer;
	IVertexBuffer*		_vBuffer;
	BBox3				_bBox;

	int					_subCount;

	ActorSub*			_subs;
	MatrixTab*			_boneMatrices;
	Matrix4				_invSkinMatrix;

	MatrixHierarchy		_initialPose;

	int					_nullNodeCount;
	ActorNullNode*		_nullNodes;

	AnimMap				_animations;

	ActorBonesNames_t	_bonesNames;

public:

	Actor(const ActorCreate_t& ac, IVideoDevice* device);
	~Actor();

	bool					isSkinned() const					{ return _skinned; }

	Material*				getMaterial() const					{ return _material; }
	IVertexBuffer*			getVertexBuffer() const				{ return _vBuffer; }
	IIndexBuffer*			getIndexBuffer() const				{ return _iBuffer; }
	const BBox3&			getBBox() const						{ return _bBox; }

	int						getSubCount() const					{ return _subCount; }

	const ActorSub*			getSubs() const						{ return _subs; }
	const ActorSub&			getSub(int n) const					{ return _subs[n]; }

	const ActorBonesNames_t&	getBonesNames() const			{ return _bonesNames; }
	int						getBoneIndex(const string& name) const;

	const MatrixTab&		getBoneMatrices() const				{ return *_boneMatrices; }
	const Matrix4&			getInvSkinMatrix() const			{ return _invSkinMatrix; }

	const MatrixHierarchy&	getInitialPose() const				{ return _initialPose; }

	int						getNullNodeCount() const			{ return _nullNodeCount; }
	const ActorNullNode*	getNullNodes() const				{ return _nullNodes; }
	const ActorNullNode&	getNullNode(int n) const			{ return _nullNodes[n]; }

	int						getNullNodeIndex(const string& name) const
	{
		for(int i=0;i<_nullNodeCount;++i)
			if(_nullNodes[i].getName()==name)
				return i;
		return -1;
	}

	int						getSubIndex(const string& name) const
	{
		for(int i=0;i<_subCount;++i)
			if(_subs[i].getName()==name)
				return i;
		return -1;
	}

	void						addAnimation(const string& animName, AnimPRSMultiTrack* anim)			{ _animations[animName]=anim; }
	const AnimPRSMultiTrack*	getAnimation(const string& animName)									{ return _animations[animName]; }
};

#endif