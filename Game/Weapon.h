
#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <StdRenderer.h>
#include <PhysicLib.h>
#include "GameMaterial.h"

class GameCharacter;
class GameObject;
class WeaponInstance;

class Weapon
{
protected:

	string					_meshName;
	string					_materialName;
	string					_fireSoundName;

	Mesh*					_rMesh;
	Material*				_rMaterial;
	ISound*					_fireSound;
	Vector3					_firePoint;

	float					_range;
	float					_damage;

	float					_fireTime;
	float					_debrisFactor;
	float					_sparkFactor;

	bool					_flashEnable;
	Color					_flashColor;
	float					_flashRange;

	void					parseToken(const string& token, ScriptFile& sf);

public:

	Weapon();
	virtual ~Weapon()		 { unload(); }

	virtual void			load();
	virtual void			unload();

	Mesh*					getMesh() const						{ return _rMesh; }
	Material*				getMaterial() const					{ return _rMaterial; }

	float					getRange() const					{ return _range; }
	float					getDamage() const					{ return _damage; }

	float					getFireTime() const					{ return _fireTime;  }
	ISound*					getFireSound() const				{ return _fireSound; }
	const Vector3&			getFirePoint() const				{ return _firePoint; }

	float					getDebrisFactor() const				{ return _debrisFactor; }
	float					getSparkFactor() const				{ return _sparkFactor; }

	bool					isFlashEnable() const				{ return _flashEnable; }
	const Color&			getFlashColor() const				{ return _flashColor; }
	float					getFlashRange() const				{ return _flashRange; }

	virtual WeaponInstance*	createInstance(GameCharacter* c)=0;

	void					emitDebris(GameMaterial* mat, const Vector3& position,
		const Vector3& normal);
};

#endif