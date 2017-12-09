
#include "stdafx.h"
#include "PhysicDynamic.h"
#include "Physic.h"
#include "PhysicUtils.h"

PhysicDynamic::PhysicDynamic(const PhysicGeometry& geo,
	const Matrix4& world, PhysicMaterial *m,
	PxPhysics* p, PxScene* s, float density) : PhysicObject(POBJ_DYNAMIC, s, m,&geo), _world(world)
{
	PMAKETRANSFORM(trans, world);

	_actor = p->createRigidDynamic(trans);
	assert(_actor);
	makeShape(geo, m, ACTORGROUP_DYNAMIC,
		ACTORGROUP_ACTOR | ACTORGROUP_AMMO);

	Vector3 center = Vector3::NullVector;
	if (geo.needPreTransform())
		center = geo.getPreTransform();

	PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)_actor), density, (PxVec3*)&center);

	_actor->userData = (IPhysicObject*)this;
	_actor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
}

PhysicDynamic::~PhysicDynamic()
{
	Physic::getSingletonRef().remDynamicFromActiveList(this);
	_actor->release();
}

void PhysicDynamic::updateWorldMatrix()
{
	PxMat44 pos(_actor->getGlobalPose());
	_world = *((Matrix4*)&pos);
}

void PhysicDynamic::addForce(const Vector3& force, bool impulse)
{
	((PxRigidBody*)_actor)->addForce(PXVEC_C(force), impulse ? PxForceMode::eIMPULSE : PxForceMode::eFORCE);
}

void PhysicDynamic::addForce(const Vector3& force, const Vector3& pos, bool impulse)
{
	PxRigidBodyExt::addForceAtPos(*((PxRigidBody*)_actor),
		PXVEC_C(force), PXVEC_C(pos),
		impulse ? PxForceMode::eIMPULSE : PxForceMode::eFORCE);
}

void PhysicDynamic::addTorque(const Vector3& torque, bool impulse)
{
	((PxRigidBody*)_actor)->addTorque(PXVEC_C(torque), impulse ? PxForceMode::eIMPULSE : PxForceMode::eFORCE);
}

