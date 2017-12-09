
#include "stdafx.h"
#include "PhysicTrigger.h"
#include "PhysicUtils.h"

PhysicTrigger::PhysicTrigger(const PhysicGeometry& geo,
	const Matrix4& world, PhysicMaterial *m,
	PxPhysics* p, PxScene* s) : PhysicObject(POBJ_TRIGGER, s, m, &geo), _world(world)
{
	PMAKETRANSFORM(trans, world);
	_actor = p->createRigidDynamic(trans);
	assert(_actor);
	((PxRigidDynamic*)_actor)->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);

	PxShape* shape=makeShape(geo, m, ACTORGROUP_TRIGGER, ACTORGROUP_ACTOR);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	_actor->userData = (IPhysicObject*)this;
}

PhysicTrigger::~PhysicTrigger()
{
	_actor->release();
}

void PhysicTrigger::setWorldMatrix(const Matrix4& world)
{
	PMAKETRANSFORM(w, world);
	((PxRigidDynamic*)_actor)->setKinematicTarget(w);
	_world = world;
}