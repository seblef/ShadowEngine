
#include "stdafx.h"
#include "WeaponGunInstance.h"
#include "WeaponGun.h"
#include "ActionServer.h"
#include "Trace.h"
#include "GameSystem.h"
#include "GameMesh.h"
#include "GameCharacter.h"
#include "Map.h"
#include <PhysicLib.h>
#include <SoundLib.h>

extern ofstream g_MediaOut;

#define GUN_MIN_COS_ANGLE_DIST			0.866f

void WeaponGunInstance::fire()
{
	if (!canFire())
		return;

	_lastFire = GameSystem::getSingletonRef().getGameTime();
	GameMaterial *mat = 0;

	RayCastInfos rc;
	rc._dir = Vector3::ZAxisVector;
	rc._dir.transformNoTranslation(_rMeshInstance->getWorldMatrix());
	rc._origin=_template->getFirePoint() * _rMeshInstance->getWorldMatrix();
	rc._range=_template->getRange();
	rc._impact = rc._origin + rc._dir * rc._range;
	rc._flags = RAYCAST_SOLID;

	IPhysicObject* obj = Physic::getSingletonRef().rayCastImpact(rc);
	if (obj)
	{
		PhysicObjectType type = obj->getObjectType();
		if (type == POBJ_GROUND)
		{
			rc._normal = Vector3::YAxisVector;
			int x = (int)rc._impact.x;
			int y = (int)rc._impact.z;
			Ground& gnd(GameSystem::getSingletonRef().getMap().getGround());

			if (x >= 0 && y >= 0 && x < gnd.getWidth() && y < gnd.getHeight())
				mat = gnd.getGameMaterial(x, y);
		}
		else if (type == POBJ_KINEMATICSCENE || type == POBJ_STATICSCENE)
			mat = ((GameMesh*)obj->getUserData())->getTemplate()->getGameMaterial();
		else if (type == POBJ_ACTOR || type == POBJ_DYNAMIC)
		{
			Hit h;
			h._dir = rc._dir;
			h._impact = rc._impact;
			h._object = (GameObject*)obj->getUserData();
			h._from = _character;
			h._damage = _template->getDamage();
			hit(h);

//			damageTarget(h, rc._origin);
		}
	}

	if (mat)
		_template->emitDebris(mat, rc._impact, rc._normal);

	TraceTemplate *t = ((WeaponGun*)_template)->getTrace();
	if (t)
		ActionServer::getSingletonRef().addAction(new Trace(*t, rc._origin, rc._impact));

	WeaponInstance::fire(rc._origin);
}

void WeaponGunInstance::damageTarget(Hit& h, const Vector3& origin) const
{
	float dirFactor=0.1f;
	Vector3 agentToTarget(((GameCharacter*)h._object)->getAgentPosition() - origin);
	agentToTarget.normalize();

	float cosDir = agentToTarget * h._dir;

	if (cosDir >= GUN_MIN_COS_ANGLE_DIST)
	{
		dirFactor += 0.9f * (cosDir - GUN_MIN_COS_ANGLE_DIST) / (1.0f - GUN_MIN_COS_ANGLE_DIST);
		h._damage = _template->getDamage() * dirFactor;
		hit(h);
	}
}