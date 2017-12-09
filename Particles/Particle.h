
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <Core.h>
#include "ParticleMaterial.h"

using namespace Core;

class ParticleRenderer;

class Particle
{
	public:

		float				_life;
		float				_mass;
		float				_friction;
		float				_percent, _percentCounter;
		
		Vector3				_pos;
		Vector3				_vel;
		Color				_color, _colorCounter;
		Vector3				_size, _sizeCounter;

		float				_distFromCam;

		ParticleMaterial*	_material;
		ParticleRenderer*	_renderer;
};


#endif