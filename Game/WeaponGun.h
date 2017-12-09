
#ifndef _WEAPONGUN_H_
#define _WEAPONGUN_H_

#include "Weapon.h"
#include "WeaponGunInstance.h"
#include "TraceTemplate.h"

class WeaponGun : public Weapon
{
protected:

	TraceTemplate*			_trace;

public:

	WeaponGun(ScriptFile& sf);
	~WeaponGun();

	TraceTemplate*			getTrace() const					{ return _trace; }

	WeaponInstance*			createInstance(GameCharacter* c)	{ return new WeaponGunInstance(this,c); }
};

#endif