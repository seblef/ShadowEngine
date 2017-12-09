
#ifndef _WEAPONROCKETINSTANCE_H_
#define _WEAPONROCKETINSTANCE_H_

#include "WeaponInstance.h"

class WeaponRocketInstance : public WeaponInstance
{
public:

	WeaponRocketInstance(Weapon* w, GameCharacter* owner);

	void				fire();
};

#endif