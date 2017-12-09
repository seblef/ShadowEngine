
#ifndef _PARTICLEEXPLOSIONEMITTER_H_
#define _PARTICLEEXPLOSIONEMITTER_H_

#include "ParticleEmitter.h"

class ParticleExplosionEmitter
{
public:

	ParticleExplosionEmitter() 							{}

	static void				emit(ParticleEmitter& e, float size, unsigned int count);
};

#endif