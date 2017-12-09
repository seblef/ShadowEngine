
#include "stdafx.h"
#include "GameEntityDynamic.h"
#include "SystemValues.h"

extern ofstream g_MediaOut;

GameEntityDynamic::GameEntityDynamic(EntityDynamic& t, const Matrix4& world) :
	GameEntity(t,world,true,DYNAMIC)
{
	_physic = Physic::getSingletonRef().createDynamic(*t.getPhysicGeometry(), world, t.getDensity());
	_physic->setUserData((GameObject*)this);
}

void GameEntityDynamic::update(float time)
{
	updateMatrix(((PhysicDynamic*)_physic)->getWorldMatrix());
}

void GameEntityDynamic::onHit(const Hit& h)
{
	Vector3 force(h._dir * h._damage * SystemValues::getSingletonRef().getDamageToForce());
	((PhysicDynamic*)_physic)->addForce(force, h._impact, true);
}