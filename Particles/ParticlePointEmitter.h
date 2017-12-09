
#ifndef _PARTICLEPOINTEMITTER_H_
#define _PARTICLEPOINTEMITTER_H_

#include "ParticleEmitter.h"
#include "ParticlePointEmitterTemplate.h"

class ParticlePointEmitter : public ParticleEmitter
{
protected:

	void				emit(Particle& p)
	{
		initParticle(p);
		p._pos=_worldMatrix;
	}

public:

	ParticlePointEmitter(ParticleRenderer *r, ParticleMaterial* m,
		unsigned int maxParticles, const Vector3& grav,
		const ParticleEmitterParams& p1,
		const ParticleEmitterParams& p2,
		float emissionRate) : ParticleEmitter(r,m,maxParticles,grav,p1,p2,emissionRate)		{}
	ParticlePointEmitter(const ParticlePointEmitterTemplate& t) :
		ParticleEmitter(t)													{}
	~ParticlePointEmitter()													{}

	const char*			getEmitterName() const								{ return "point"; }
};

#endif