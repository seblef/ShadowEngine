
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "Action.h"
#include "ExplosionTemplate.h"

class Explosion : public Action
{
protected:

	const ExplosionTemplate*	_template;
	Vector3						_position;
	bool						_exploded;

	Particles*					_rParticles;

	ParticlePointEmitter*		_explosionEmitter;
	ParticlePointEmitter*		_smokeEmitter;

	float						_smokeTime;

	void						explode();

public:

	Explosion(const ExplosionTemplate& e, const Vector3& pos);
	~Explosion();

	void						update(float time);
};

#endif