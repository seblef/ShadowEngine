
#include "stdafx.h"
#include "PhysicGround.h"
#include "PhysicUtils.h"

PhysicGround::PhysicGround(PxPhysics* physic, PxScene* scene, PhysicMaterial* mat,
	int width, int height) :
	PhysicObject(POBJ_GROUND,scene,mat)
{
	float sw = 0.5f * (float)width;
	float sh = 0.5f * (float)height;

	//			Hack
	Matrix4 world;
	world.createTranslate(sw, -0.5f, sh);
	PMAKETRANSFORM(t, world);

	_actor=physic->createRigidStatic(t);
	PxShape *s = _actor->createShape(PxBoxGeometry(
		sw + 0.5f, 1.0f,sh + 0.5f),
		*mat->getMaterial());

	PxFilterData filter;
	filter.word0 = ACTORGROUP_SCENE;
	filter.word1 = ACTORGROUP_AMMO;
	s->setQueryFilterData(filter);

	_actor->userData = (IPhysicObject*)this;
}

PhysicGround::~PhysicGround()
{
	_actor->release();
}