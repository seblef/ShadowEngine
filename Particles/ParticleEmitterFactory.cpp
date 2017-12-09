
#include "stdafx.h"
#include "ParticleEmitterFactory.h"
#include "ParticlePointEmitter.h"


ParticleEmitter* ParticleEmitterFactory::createParticleEmitter(const string& emitterName,
															   ParticleRenderer* r, ParticleMaterial *m,
															   unsigned int maxParticles, const Vector3& grav,
															   const ParticleEmitterParams& p1, const ParticleEmitterParams& p2,
															   float emissionRate)
{
	return new ParticlePointEmitter(r,m,maxParticles,grav,p1,p2,emissionRate);
}

ParticleEmitter* ParticleEmitterFactory::createParticleEmitter(const ParticleEmitterTemplate& t)
{
	return new ParticlePointEmitter(*((const ParticlePointEmitterTemplate*)&t));
}