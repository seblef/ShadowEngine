
#ifndef _TEMPLATEPARTICLESYSTEM_H_
#define _TEMPLATEPARTICLESYSTEM_H_

#include <ParticleLib.h>

class TemplateParticleSystem
{
protected:

	ParticleSystemTemplate*			_template;

public:

	TemplateParticleSystem(ParticleSystemTemplate* t) : _template(t)			{}
	~TemplateParticleSystem()
	{
		delete _template;
	}

	ParticleSystemTemplate*			getTemplate() const					{ return _template; }
};

#endif