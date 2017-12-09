
#ifndef _G_PARTICLESTEMPLATE_H_
#define _G_PARTICLESTEMPLATE_H_

#include <ParticleLib.h>

class G_ParticlesTemplate
{
protected:

	ParticleSystemTemplate*		_template;

	string						_name;

public:

	G_ParticlesTemplate(ParticleSystemTemplate* t, const string& name) : _template(t), _name(name)		{}
	~G_ParticlesTemplate()
	{
		delete _template;
	}

	void						setName(const string& name)				{ _name = name; }
	const string&				getName() const							{ return _name; }

	ParticleSystemTemplate&		getTemplate()							{ return *_template; }

	void						setNewTemplate(ParticleSystemTemplate* t)
	{
		delete _template;
		_template = t;
	}
};

#endif