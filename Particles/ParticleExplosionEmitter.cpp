
#include "stdafx.h"
#include "ParticleExplosionEmitter.h"

const float ParticleExplosionVelocityFactor = 2.5f;

void ParticleExplosionEmitter::emit(ParticleEmitter& e, float width, unsigned int count)
{
	Particle *p;
	Vector3 vel;

	for (unsigned int i = 0; i < count; ++i)
	{
		p = e.getNextParticle();
		if (!p)
			return;

		float rot_xy = RAND * 2.0f * M_PI;
		float rot_xz = RAND * 2.0f * M_PI;
		float c_rot_xz = cosf(rot_xz);
		float s_rot_xz = sinf(rot_xz);
		float c_rot_xy = cosf(rot_xy);
		float s_rot_xy = sinf(rot_xy);

		vel.x = s_rot_xy * c_rot_xz;
		vel.y = c_rot_xy * c_rot_xz;
		vel.z = s_rot_xz;
		vel *= ParticleExplosionVelocityFactor;

		float size = RAND * width * 2.0f + 2.0f;
		e.getParameters1().setEndSize(Vector3(size, size, size));

		e.initParticle(*p);

		p->_vel = vel;
		p->_pos = e.getWorldMatrix();
	}
}