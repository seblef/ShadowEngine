
#include "stdafx.h"
#include "Trace.h"
#include "TraceTemplate.h"
#include "GameCamera.h"


Trace::Trace(const TraceTemplate& t, const Vector3& p1, const Vector3& p2)
{
	ParticleSystem *ps = new ParticleSystem(*t.getParticleSystem());
	float count = (p1 - p2).getLength() * t.getDensity();

	Matrix4 world;
	world.createTranslate(p1.x, p1.y, p1.z);
	
	for (int i = 0; i < ps->getEmitterCount(); ++i)
	{
		ps->setWorldMatrix(world);
		ParticleLinearEmitter::emit(*ps->getEmitter(i), p1, p2, (unsigned int)count);
	}

	ps->countParticles();

	_particleSystem = new Particles(ps, world, true);
	_particleSystem->wakeUp();
	Renderer::getSingletonRef().addRenderable(_particleSystem);
}

Trace::~Trace()
{
	_particleSystem->sleep();
	Renderer::getSingletonRef().remRenderable(_particleSystem);
	delete _particleSystem;
}

void Trace::update(float time)
{
	_dead = _particleSystem->getParticleSystem()->getParticleCount() == 0;
}