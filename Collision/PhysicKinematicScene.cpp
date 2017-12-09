
#include "stdafx.h"
#include "PhysicKinematicScene.h"
#include "PhysicUtils.h"

PhysicKinematicScene::PhysicKinematicScene(const PhysicGeometry& geo,
	const Matrix4& world, PhysicMaterial *m,
	PxPhysics* p, PxScene* s) : PhysicObject(POBJ_KINEMATICSCENE, s, m, &geo), _world(world)
{
	PMAKETRANSFORM(trans, world);

	_actor = p->createRigidDynamic(trans);
	assert(_actor);

	((PxRigidDynamic*)_actor)->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);
	makeShape(geo, m, ACTORGROUP_SCENE,
		ACTORGROUP_AMMO);

	_actor->userData = (IPhysicObject*)this;
}

PhysicKinematicScene::~PhysicKinematicScene()
{
	_actor->release();
}

void PhysicKinematicScene::setWorldMatrix(const Matrix4& world)
{
	PMAKETRANSFORM(w, world);
	((PxRigidDynamic*)_actor)->setKinematicTarget(w);
	_world = world;
}