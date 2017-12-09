
#ifndef _WEAPONINSTANCE_H_
#define _WEAPONINSTANCE_H_

#include "Weapon.h"
#include "Hit.h"

class GameCharacter;

class WeaponInstance
{
protected:

	Weapon*					_template;
	MeshInstance*			_rMeshInstance;
	GameCharacter*			_character;

	double					_lastFire;

	void					hit(const Hit& hit) const;
	void					fire(const Vector3& origin) const;

public:

	WeaponInstance(Weapon* w, GameCharacter *c);
	virtual ~WeaponInstance();

	Weapon*					getTemplate() const		{ return _template; }
	MeshInstance*			getMeshInstance() const	{ return _rMeshInstance; }

	virtual void			fire()=0;

	bool					canFire() const;
};

#endif