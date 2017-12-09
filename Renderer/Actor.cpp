
#include "stdafx.h"
#include "Actor.h"

Actor::Actor(const ActorCreate_t& ac, IVideoDevice* device) : _initialPose(*ac.initialPose), _skinned(ac.skinned),
	_material(ac.material), _bBox(ac.box), _subCount(ac.subCount), _subs(ac.subs), _invSkinMatrix(ac.invSkinMatrix),
	_boneMatrices(ac.boneMatrices), _nullNodeCount(ac.nullNodeCount), _nullNodes(ac.nullNodes)
{
	memcpy(&_bonesNames, &ac.bonesNames, sizeof(ActorBonesNames_t));

	_vBuffer=device->createVertexBuffer(ac.vxCount,_skinned ? VX_3DSKIN : VX_3D,BU_IMMUTABLE,ac.vertices);
	_iBuffer=device->createIndexBuffer(ac.triCount*3,IDX_16BITS,BU_IMMUTABLE,ac.triangles);
}

Actor::~Actor()
{
	delete _vBuffer;
	delete _iBuffer;

	if(_subs)			delete[] _subs;
	if(_nullNodes)		delete[] _nullNodes;
	if(_boneMatrices)	delete	_boneMatrices;

	AnimMap::iterator a(_animations.begin());
	for(;a!=_animations.end();++a)
		delete a->second;
}

int Actor::getBoneIndex(const string& boneName) const
{
	if (_boneMatrices)
	{
		for (int i = 0; i < _boneMatrices->getCount(); ++i)
		{
			if (boneName == _bonesNames.bonesNames[i])
				return i;
		}
	}

	return -1;
}