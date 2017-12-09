
#ifndef _PARTICLEEMITTERFACTORY_H_
#define _PARTICLEEMITTERFACTORY_H_

#include "ParticleEmitter.h"

class ParticleEmitterFactory
{
public:

	static ParticleEmitter*			createParticleEmitter(const string& emitterName,
															   ParticleRenderer* r, ParticleMaterial *m,
															   unsigned int maxParticles, const Vector3& grav,
															   const ParticleEmitterParams& p1, const ParticleEmitterParams& p2,
															   float emissionRate);
	static ParticleEmitter*			createParticleEmitter(const ParticleEmitterTemplate& t);
};

#endif