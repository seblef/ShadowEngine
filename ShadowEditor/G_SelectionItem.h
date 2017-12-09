
#ifndef _G_SELECTIONITEM_H_
#define _G_SELECTIONITEM_H_

#include <PxPhysicsAPI.h>
#include <Core.h>

using namespace Core;
using namespace physx;

#define PMAKETRANSFORM(t, m)					PxTransform t(*((PxMat44*)&m)); if(!t.isValid()) t.q.normalize();


class G_SelectionItem
{
protected:

	PxRigidDynamic*				_actor;
	PxScene*					_scene;

public:

	G_SelectionItem(PxRigidDynamic* actor, PxScene* scene) : _actor(actor), _scene(scene)			{}
	~G_SelectionItem()
	{
		_actor->release();
	}

	void						setUserData(void* d)						{ _actor->userData=d; }
	void*						getUserData() const							{ return _actor->userData; }

	void						updateWorldMatrix(const Matrix4& w)
	{
		PMAKETRANSFORM(t,w)
		_actor->setKinematicTarget(t);
	}

	void						addToScene()								{ _scene->addActor(*_actor); }
	void						remFromScene()								{ _scene->removeActor(*_actor); }
};

#endif