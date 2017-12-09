
#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include <ParticleLib.h>
#include "Renderable.h"
#include "IUpdatable.h"

class Particles : public Renderable, public IUpdatable
{
protected:

	ParticleSystem*				_system;

public:

	Particles(ParticleSystem *system, const Matrix4& world, bool alwaysVisible=false);
	~Particles();

	void						setWorldMatrix(const Matrix4& world);
	void						enqueue();
	void						update(float time);

	ParticleSystem*				getParticleSystem() const					{ return _system; }
};

#endif