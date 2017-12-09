
#include "stdafx.h"
#include "PhysicAmmo.h"
#include "PhysicUtils.h"

PhysicAmmo::PhysicAmmo(const PhysicGeometry& geo,
	const Matrix4& world, PhysicMaterial *m,
	PxPhysics* p, PxScene* s) : PhysicObject(POBJ_AMMO, s, m, &geo), _world(world)
{
	PMAKETRANSFORM(trans, world);

	_actor = p->createRigidDynamic(trans);
	assert(_actor);

	((PxRigidDynamic*)_actor)->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);
	makeShape(geo, m, ACTORGROUP_AMMO,
		ACTORGROUP_ACTOR | ACTORGROUP_DYNAMIC | ACTORGROUP_SCENE);

	_actor->userData = (IPhysicObject*)this;
}

PhysicAmmo::~PhysicAmmo()
{
	_actor->release();
}

void PhysicAmmo::setWorldMatrix(const Matrix4& world)
{
	PMAKETRANSFORM(w, world);
	((PxRigidDynamic*)_actor)->setKinematicTarget(w);
	_world = world;
}