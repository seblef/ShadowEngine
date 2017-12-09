
#include "stdafx.h"
#include "Explosion.h"
#include "Debris.h"
#include "ActionServer.h"
#include "FieldHit.h"
#include "Flash.h"
#include <SoundLib.h>

const float ExplosionFlashAscend = 0.1f;
const float ExplosionFlashStable = 0.1f;
const float ExplosionFlashDescend = 0.1f;

Explosion::Explosion(const ExplosionTemplate& e, const Vector3& pos) : _template(&e),
	_explosionEmitter(0), _smokeEmitter(0), _rParticles(0), _smokeTime(0),
	_position(pos), _exploded(false)
{
}

Explosion::~Explosion()
{
	if (_rParticles)
	{
		_rParticles->sleep();
		Renderer::getSingletonRef().remRenderable(_rParticles);
		delete _rParticles;
	}
}

void Explosion::update(float time)
{
	if (!_exploded)
	{
		explode();
		_exploded = true;
	}

	_smokeTime += time;
	if (_smokeEmitter && _smokeTime >= _template->getSmokeLife())
		_smokeEmitter->setEmissionRate(0);

	_dead = _rParticles->getParticleSystem()->getParticleCount() <= 0;
}

void Explosion::explode()
{
	Matrix4 world;
	world.createTranslate(_position.x, _position.y, _position.z);

	if (_template->getParticles()->getEmitterCount() > 0)
	{
		ParticleSystem* ps = new ParticleSystem(*_template->getParticles());
		ps->setWorldMatrix(world);
		_rParticles = new Particles(ps, world, true);

		if (_template->getExplosionEmitter() != -1)
		{
			_explosionEmitter = (ParticlePointEmitter*)ps->getEmitter(_template->getExplosionEmitter());
			ParticleExplosionEmitter::emit(*_explosionEmitter, _template->getExplosionSize(),
				_template->getExplosionParticleCount());
		}

		if (_template->getSmokeEmitter() != -1)
			_smokeEmitter = (ParticlePointEmitter*)ps->getEmitter(_template->getSmokeEmitter());

		ps->countParticles();
		_rParticles->wakeUp();

		Renderer::getSingletonRef().addRenderable(_rParticles);
	}

	if (_template->getDebrisMesh() && _template->getDebrisCount() > 0)
	{
		ActionServer::getSingletonRef().addAction(new Debris(_template->getDebrisMesh(),
			_template->getDebrisCount(), _position, Vector3::YAxisVector,
			_template->getDebrisMaxAngle(), _template->getDebrisMinVelocity(),
			_template->getDebrisMaxVelocity(),
			_template->getDebrisMaxAngleSpeed(), _template->getDebrisSize()
			));
	}

	if (_template->getExplosionSound())
	{
		ISoundVirtualSource *src = SoundSystem::getSingletonRef().play(
			_template->getExplosionSound(), 1, 10.0f)->getSource();
		src->setPosition(_position);
		src->setInRadius(50.0f);
	}

	if (_template->getHitEnvironment())
	{
		FieldHit::hitField(_position,
			_template->getInnerRadius(),
			_template->getOuterRadius(),
			_template->getInnerDamage());
	}

	ActionServer::getSingletonRef().addAction(new Flash(_template->getExplosionColor(),
		ExplosionFlashAscend, ExplosionFlashStable, ExplosionFlashDescend,
		_template->getOuterRadius() * 1.5f,
		_position));
}