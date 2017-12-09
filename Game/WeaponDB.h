
#ifndef _WEAPONDB_H_
#define _WEAPONDB_H_

#include "Weapon.h"

class WeaponDB : public TDataBase<Weapon>
{
public:

	WeaponDB(const string& weaponFile);

	void				unloadWeapons();
};

#endif