
#ifndef _PARTICLEEMITTERTEMPLATEFACTORY_H_
#define _PARTICLEEMITTERTEMPLATEFACTORY_H_

#include "ParticleEmitterTemplate.h"

class ParticleEmitterTemplateFactory
{
public:

	ParticleEmitterTemplateFactory()			{}

	static ParticleEmitterTemplate*				createTemplate(const string& tName, ScriptFile& sf);
	static ParticleEmitterTemplate*				createTemplate(const string& tName, const string& renderer,
		ParticleMaterial* mat, const ParticleEmitterParams& p1, const ParticleEmitterParams& p2,
		unsigned int maxParticles, const Vector3& gravity, float emissionRate);
};

#endif