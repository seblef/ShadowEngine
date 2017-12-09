
#ifndef _PARTICLEEMITTER_H_
#define _PARTICLEEMITTER_H_

#include "ParticleEmitterTemplate.h"
#include "ParticleRenderer.h"
#include "Particle.h"



class ParticleEmitter
{
protected:

	ParticleRenderer*					_renderer;
	ParticleMaterial*					_material;

	ParticleEmitterParams				_params1, _params2;

	unsigned int						_particleCount;
	unsigned int						_maxParticles;

	Particle*							_particles;
	Particle**							_freeParticles;
	Particle**							_usedParticles;

	Vector3								_gravity;
	float								_emissionRate;
	float								_emissionCounter;

	Matrix4								_worldMatrix;
	BBox3								_worldBBox;

	virtual void						initEmitter();
	virtual void						emit(Particle& p) = 0;

public:

	ParticleEmitter(ParticleRenderer *r, ParticleMaterial* m,
		unsigned int maxParticles, const Vector3& grav,
		const ParticleEmitterParams& p1,
		const ParticleEmitterParams& p2,
		float emissionRate);
	ParticleEmitter(const ParticleEmitterTemplate& t);
	~ParticleEmitter();

	virtual const char*					getEmitterName() const							{ return "null"; }

	void								setWorldMatrix(const Matrix4& w)				{ _worldMatrix = w; }
	const Matrix4&						getWorldMatrix() const							{ return _worldMatrix; }
	const BBox3&						getWorldBBox() const							{ return _worldBBox; }

	virtual void						update(float time, const Camera& c);

	unsigned int						getParticleCount() const						{ return _particleCount; }

	void								collectParticles(Particle** p, unsigned int& count, unsigned int max_count);

	ParticleEmitterParams&				getParameters1()								{ return _params1; }
	ParticleEmitterParams&				getParameters2()								{ return _params2; }

	unsigned int						getMaxParticles() const							{ return _maxParticles; }
	float								getEmissionRate() const							{ return _emissionRate; }
	const Vector3&						getGravity() const								{ return _gravity; }
	Vector3&							getGravity()									{ return _gravity; }

	ParticleRenderer*					getRenderer() const								{ return _renderer; }
	ParticleMaterial*					getMaterial() const								{ return _material; }

	void								setMaxParticles(unsigned int mp);
	void								setEmissionRate(float rate)						{ _emissionRate = rate; }
	void								setGravity(const Vector3& g)					{ _gravity = g; }

	void								setRenderer(ParticleRenderer* r);
	void								setMaterial(ParticleMaterial* m);

	Particle*							getNextParticle();
	void								initParticle(Particle& p) const;
};

#endif