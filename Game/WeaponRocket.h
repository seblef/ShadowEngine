
#ifndef _WEAPONROCKET_H_
#define _WEAPONROCKET_H_

#include "Weapon.h"
#include "TemplateMesh.h"
#include "ExplosionTemplate.h"
#include <ParticleLib.h>

class WeaponRocket : public Weapon
{
protected:

	//				Ammo
	string					_rocketMeshFile;
	string					_rocketMaterialFile;
	string					_rocketSoundFile;
	float					_rocketLife;
	float					_rocketVelocity;

	TemplateMesh*			_rocketMesh;
	ISound*					_rocketSound;

	//				Effects
	string					_rocketExplosionName;
	ExplosionTemplate*		_rocketExplosion;

	Vector3					_rocketSmokePoint;
	ParticleSystemTemplate*	_rocketSmoke;

	bool					_loaded;

public:

	WeaponRocket(ScriptFile& sf);
	~WeaponRocket();

	void					load();
	void					unload();

	TemplateMesh*			getRocketMesh() const				{ return _rocketMesh; }
	ISound*					getRocketSound() const				{ return _rocketSound; }
	ExplosionTemplate*		getRocketExplosion() const			{ return _rocketExplosion; }
	ParticleSystemTemplate*	getRocketSmoke() const				{ return _rocketSmoke; }
	const Vector3&			getRocketSmokePoint() const			{ return _rocketSmokePoint; }

	float					getRocketLife() const				{ return _rocketLife; }
	float					getRocketVelocity() const			{ return _rocketVelocity; }

	WeaponInstance*			createInstance(GameCharacter* c);
};
#endif