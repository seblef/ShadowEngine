
#include "stdafx.h"
#include "WeaponRocketInstance.h"
#include "WeaponRocket.h"
#include "Rocket.h"
#include "ActionServer.h"
#include "GameSystem.h"

WeaponRocketInstance::WeaponRocketInstance(Weapon* w, GameCharacter* owner) : WeaponInstance(w,owner)
{
	w->load();
}

void WeaponRocketInstance::fire()
{
	if (canFire())
	{
		_lastFire = GameSystem::getSingletonRef().getGameTime();

		Matrix4 world(_rMeshInstance->getWorldMatrix());
		
		Vector3 dir(Vector3::ZAxisVector);
		dir.transformNoTranslation(world);
		Vector3 origin(_template->getFirePoint());
		origin *= world;

		world(3, 0) = origin.x;
		world(3, 1) = origin.y;
		world(3, 2) = origin.z;

		ActionServer::getSingletonRef().addAction(new Rocket((WeaponRocket*)_template, _character,
			world, dir));

		WeaponInstance::fire(origin);
	}
}