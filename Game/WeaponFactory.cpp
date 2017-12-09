
#include "stdafx.h"
#include "WeaponFactory.h"
#include "WeaponGun.h"
#include "WeaponRocket.h"


Weapon* WeaponFactory::loadWeapon(const string& className, ScriptFile& sf)
{
	Weapon* w=0;
	if (className == "gun")
		w = new WeaponGun(sf);
	else if (className == "rocket")
		w = new WeaponRocket(sf);

	return w;
}