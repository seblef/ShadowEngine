
#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "Ammo.h"
#include <StdRenderer.h>
#include <PhysicLib.h>
#include <ParticleLib.h>
#include <SoundLib.h>

class WeaponRocket;
class GameCharacter;

class Rocket : public Ammo
{
protected:

	WeaponRocket*			_weapon;
	GameCharacter*			_owner;

	Matrix4					_world;
	Matrix4					_smokeWorld;

	Vector3					_velocity;
	float					_life;

	MeshInstance*			_rocketMesh;
	Light*					_rocketLight;
	IPhysicObject*			_rocketPhysic;
	Particles*				_rocketSmoke;

	SoundVirtualSource		_rocketSource;

	void					explode();
	void					cleanUp();

public:

	Rocket(WeaponRocket* weapon, GameCharacter* owner,
		const Matrix4& originalWorld, const Vector3& direction);
	~Rocket();

	void					update(float time);

	void					onContact(IPhysicObject* other);
};

#endif