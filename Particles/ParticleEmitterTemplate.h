
#ifndef _PARTICLEEMITTERTEMPLATE_H_
#define _PARTICLEEMITTERTEMPLATE_H_

#include "ParticleEmitterParams.h"
#include "ParticleMaterial.h"

class ParticleEmitterTemplate
{
protected:

	string								_emitterName;

	string								_renderer;
	ParticleMaterial*					_material;

	ParticleEmitterParams				_params1, _params2;

	unsigned int						_maxParticles;

	Vector3								_gravity;
	float								_emissionRate;

	void								parseCommon(ScriptFile& sf);

public:

	ParticleEmitterTemplate(const char *emitterName) : _material(0), _maxParticles(10),
		_gravity(Vector3::NullVector), _emissionRate(0), _emitterName(emitterName) {}
	ParticleEmitterTemplate(const char *emitterName, const string& renderer,
		ParticleMaterial* mat,
		const ParticleEmitterParams& p1,
		const ParticleEmitterParams& p2,
		unsigned int maxParticles,
		const Vector3& gravity,
		float emissionRate) : _emitterName(emitterName), _renderer(renderer),
		_material(mat), _params1(p1), _params2(p2),
		_maxParticles(maxParticles), _gravity(gravity), _emissionRate(emissionRate)		{}
	~ParticleEmitterTemplate();

	const string&						getEmitterName() const			{ return _emitterName; }

	const string&						getRenderer() const				{ return _renderer; }
	ParticleMaterial*					getMaterial() const				{ return _material; }

	const ParticleEmitterParams&		getParameters1() const			{ return _params1; }
	const ParticleEmitterParams&		getParameters2() const			{ return _params2; }

	unsigned int						getMaxParticles() const			{ return _maxParticles; }

	const Vector3&						getGravity() const				{ return _gravity;  }

	float								getEmissionRate() const			{ return _emissionRate; }
};

#endif