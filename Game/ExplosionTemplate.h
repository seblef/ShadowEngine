
#ifndef _EXPLOSIONTEMPLATE_H_
#define _EXPLOSIONTEMPLATE_H_

#include "Effect.h"
#include "TemplateMesh.h"
#include <ParticleLib.h>

using namespace Core;

class ExplosionTemplate : public Effect
{
protected:

	float							_size;
	float							_life;
	Color							_color;
	bool							_explosionColorBlend;
	int								_particleCount;

	float							_smokeDensity;
	float							_smokeLife;
	bool							_smokeColorBlend;

	string							_explosionTextureDir;
	string							_smokeTexture;

	string							_debrisMeshFile;
	string							_debrisMaterial;
	float							_debrisSize;
	float							_debrisMinVelocity;
	float							_debrisMaxVelocity;
	float							_debrisMaxAngle;
	float							_debrisMaxAngleSpeed;
	int								_debrisCount;

	ParticleSystemTemplate*			_particles;

	TemplateMesh*					_debrisMesh;

	bool							_loaded;
	int								_explosionEmitter;
	int								_smokeEmitter;

	string							_explosionSoundFile;
	ISound*							_explosionSound;

	bool							_hitEnvironment;
	float							_innerRadius;
	float							_outerRadius;
	float							_innerDamage;

public:

	ExplosionTemplate(ScriptFile& sf);
	~ExplosionTemplate();

	void							load();
	void							unload();

	ParticleSystemTemplate*			getParticles() const				{ return _particles; }
	int								getExplosionEmitter() const			{ return _explosionEmitter; }
	int								getSmokeEmitter() const				{ return _smokeEmitter; }

	float							getSmokeLife() const				{ return _smokeLife; }

	float							getExplosionSize() const			{ return _size; }
	int								getExplosionParticleCount() const	{ return _particleCount; }
	const Color&					getExplosionColor() const			{ return _color; }

	TemplateMesh*					getDebrisMesh() const				{ return _debrisMesh; }
	float							getDebrisSize() const				{ return _debrisSize; }
	float							getDebrisMinVelocity() const		{ return _debrisMinVelocity; }
	float							getDebrisMaxVelocity() const		{ return _debrisMaxVelocity; }
	float							getDebrisMaxAngle() const			{ return _debrisMaxAngle; }
	float							getDebrisMaxAngleSpeed() const		{ return _debrisMaxAngleSpeed; }
	int								getDebrisCount() const				{ return _debrisCount; }

	ISound*							getExplosionSound() const			{ return _explosionSound; }

	bool							getHitEnvironment() const			{ return _hitEnvironment; }
	float							getInnerRadius() const				{ return _innerRadius; }
	float							getOuterRadius() const				{ return _outerRadius; }
	float							getInnerDamage() const				{ return _innerDamage; }
};

#endif