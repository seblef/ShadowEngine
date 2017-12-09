
#ifndef _ENTITYEXPLOSIVE_H_
#define _ENTITYEXPLOSIVE_H_

#include "EntityDynamic.h"

class ExplosionTemplate;

class EntityExplosive : public EntityDynamic
{
protected:

	string				_explosionName;
	float				_damageThreshold;

	ExplosionTemplate*	_explosionTemplate;
	
public:

	EntityExplosive(ScriptFile& sf);
	~EntityExplosive();

	void				load();

	GameEntity*			createInstance(const Matrix4& world);

	ExplosionTemplate*	getExplosion() const						{ return _explosionTemplate; }
	float				getDamageThreshold() const					{ return _damageThreshold; }
};

#endif