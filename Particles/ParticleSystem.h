
#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "ParticleEmitter.h"
#include "ParticleSystemTemplate.h"

class ParticleSystem
{
protected:

	int						_emitterCount;
	ParticleEmitter**		_emitters;
	bool					_autoDeleteEmitters;		//		For editing purposes

	BBox3					_worldBBox;

	unsigned int			_particleCount;

public:

	ParticleSystem(int emitterCount, ParticleEmitter** _emitters, bool autoDeleteEmitters=true);
	ParticleSystem(const ParticleSystemTemplate& t);
	~ParticleSystem();

	void					setWorldMatrix(const Matrix4& w);
	const BBox3&			getWorldBBox() const							{ return _worldBBox; }

	void					update(float time, const Camera& c);

	void					collectParticles(Particle **p, unsigned int& count, unsigned int max_count);
	unsigned int			getParticleCount() const						{ return _particleCount; }

	int						getEmitterCount() const							{ return _emitterCount; }
	ParticleEmitter*		getEmitter(int n) const							{ return _emitters[n]; }

	void					countParticles();
};

#endif