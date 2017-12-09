
#ifndef _TRACETEMPLATE_H_
#define _TRACETEMPLATE_H_

#include <ParticleLib.h>

class TraceTemplate
{
protected:

	ParticleSystemTemplate*			_particleSystem;
	float							_density;

public:

	TraceTemplate(ScriptFile& sf);
	TraceTemplate(ParticleSystemTemplate* ps, float density) :
		_particleSystem(ps), _density(density)						{}
	~TraceTemplate();

	ParticleSystemTemplate*			getParticleSystem() const		{ return _particleSystem; }
	float							getDensity() const				{ return _density;  }
};

#endif