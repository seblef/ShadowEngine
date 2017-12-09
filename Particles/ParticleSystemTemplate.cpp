
#include "stdafx.h"
#include "ParticleSystemTemplate.h"
#include "ParticleEmitterTemplateFactory.h"

ParticleSystemTemplate::ParticleSystemTemplate(ScriptFile& sf)
{
	parse(sf);
}

ParticleSystemTemplate::ParticleSystemTemplate(const string& psFile)
{
	ScriptFile sf(psFile);
	if (sf.good())
		parse(sf);
}

ParticleSystemTemplate::ParticleSystemTemplate(int emitterCount, ParticleEmitterTemplate** emitters)
{
	for (int i = 0; i < emitterCount; ++i)
		_emitters.push_back(emitters[i]);
}

ParticleSystemTemplate::~ParticleSystemTemplate()
{
	EmitterVector::iterator e(_emitters.begin());
	for (; e != _emitters.end(); ++e)
		delete *e;
}

void ParticleSystemTemplate::parse(ScriptFile& sf)
{
	string token(sf.getToken());
	while (sf.good() && token != "end_system")
	{
		if (token == "emitter")
		{
			_emitters.push_back(ParticleEmitterTemplateFactory::createTemplate(
				sf.getToken(), sf));
		}
		token = sf.getToken();
	}
}