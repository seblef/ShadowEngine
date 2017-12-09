
#include "stdafx.h"
#include "ParticleLinearEmitter.h"

void ParticleLinearEmitter::emit(ParticleEmitter& e,
	const Vector3& p1, const Vector3& p2, unsigned int count)
{
	Vector3 diff(p2-p1);

	for(unsigned int i=0;i<count;++i)
	{
		Particle *p=e.getNextParticle();
		if(p)
		{
			float t=RAND;
			e.initParticle(*p);
			p->_pos=p1 + diff*t;
		}
	}
}