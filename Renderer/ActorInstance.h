
#ifndef _ACTORINSTANCE_H_
#define _ACTORINSTANCE_H_

#include "Actor.h"
#include "Renderable.h"
#include "IUpdatable.h"
#include "ActorAnimInstance.h"
#include "MeshInstance.h"

class ActorInstance : public Renderable, public IUpdatable
{
protected:

	Actor*						_model;
	ActorAnimInstance			_animation;
	MatrixTab					_pose;
	MatrixHierarchy				_worldMatrices;
	MatrixTab*					_boneMatrices;
	MeshInstance**				_nullNodesMeshes;

public:

	ActorInstance(Actor* model, const Matrix4& world, bool alwaysVisible);
	~ActorInstance()
	{
		if(_boneMatrices)			delete _boneMatrices;
	}

	const Actor*			getModel() const					{ return _model; }

	MatrixTab&				getPose()							{ return _pose; }
	const MatrixTab&		getPose() const						{ return _pose; }
	const MatrixHierarchy&	getWorldMatrices() const			{ return _worldMatrices; }
	MatrixHierarchy&		getWorldMatrices()					{ return _worldMatrices; }
	const MatrixTab*		getBoneMatrices() const				{ return _boneMatrices; }

	void					enqueue();
	void					update(float time);

	void					playAnimation(const string& animName,
		bool loop=true, bool reverse=false)							{ _animation.play(_model->getAnimation(animName),_pose,loop,reverse); }
	void					stopAnimation()							{ _animation.stop(); }
	void					applyOriginalPose()						{ _pose=_model->getInitialPose(); }

	void					attachMeshToNullNode(MeshInstance *m, int nn)
	{
		assert(nn >= 0 && nn < _model->getNullNodeCount());
		_nullNodesMeshes[nn]=m;
	}
	void					detachMeshToNullNode(int nn)
	{
		assert(nn >= 0 && nn < _model->getNullNodeCount());
		_nullNodesMeshes[nn]=0;
	}

	void					attachMeshToNullNode(MeshInstance* m, const string& nullNodeName)
	{
		attachMeshToNullNode(m,_model->getNullNodeIndex(nullNodeName));
	}
	void					detachMeshToNullNode(const string& nullNodeName)
	{
		detachMeshToNullNode(_model->getNullNodeIndex(nullNodeName));
	}
};

#endif