
#ifndef _G_PARTICLESYSTEM_H_
#define _G_PARTICLESYSTEM_H_

#include "G_Object.h"
#include "G_ParticlesTemplate.h"

class G_ParticleSystem : public G_Object
{
protected:

	G_ParticlesTemplate*		_template;

public:

	G_ParticleSystem(G_ParticlesTemplate* t);
	G_ParticleSystem(const G_ParticleSystem& ps);
	~G_ParticleSystem()												{}

	G_ParticlesTemplate*		getTemplate() const					{ return _template; }

	G_Object*					copy()								{ return new G_ParticleSystem(*this); }

	void						onAddToScene();
	void						onRemFromScene();

	void						drawSelection(G_EditorObjDrawer* d);
};

#endif