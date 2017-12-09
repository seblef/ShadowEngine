
#include "stdafx.h"
#include "PhysicStaticScene.h"
#include "PhysicUtils.h"

PhysicStaticScene::PhysicStaticScene(const PhysicGeometry& geo,
	const Matrix4& world, PhysicMaterial *m,
	PxPhysics* p, PxScene* s) : PhysicObject(POBJ_STATICSCENE, s, m, &geo)
{
	PMAKETRANSFORM(trans, world)
	_actor = p->createRigidStatic(trans);
	assert(_actor);

	makeShape(geo, m, ACTORGROUP_SCENE, ACTORGROUP_AMMO);

	_actor->userData = (IPhysicObject*)this;
}

PhysicStaticScene::~PhysicStaticScene()
{
	_actor->release();
}