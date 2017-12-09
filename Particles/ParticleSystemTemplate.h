
#ifndef _PARTICLESYSTEMTEMPLATE_H_
#define _PARTICLESYSTEMTEMPLATE_H_

#include "ParticleEmitterTemplate.h"

class ParticleSystemTemplate
{
protected:

	typedef vector<ParticleEmitterTemplate*> EmitterVector;
	EmitterVector				_emitters;

	void						parse(ScriptFile& sf);

public:

	ParticleSystemTemplate(ScriptFile& sf);
	ParticleSystemTemplate(const string& psFile);
	ParticleSystemTemplate(int emitterCount, ParticleEmitterTemplate** emitters);
	~ParticleSystemTemplate();

	unsigned int				getEmitterCount() const				{ return _emitters.size(); }
	ParticleEmitterTemplate*	getEmitter(int n) const				{ return _emitters[n]; }
};

#endif