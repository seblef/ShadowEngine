
#ifndef _WEAPONFACTORY_H_
#define _WEAPONFACTORY_H_

#include "Weapon.h"

class WeaponFactory
{
public:

	WeaponFactory()				{}

	static Weapon*				loadWeapon(const string& className, ScriptFile& sf);
};

#endif