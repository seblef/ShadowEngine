
#include "stdafx.h"
#include "ParticleSystem.h"
#include "ParticleEmitterFactory.h"

ParticleSystem::ParticleSystem(int emitterCount, ParticleEmitter** emitters, bool autoDeleteEmitters) : _emitterCount(emitterCount), _emitters(emitters),
	_worldBBox(BBox3::BoundBox), _particleCount(0), _autoDeleteEmitters(autoDeleteEmitters)
{
}

ParticleSystem::ParticleSystem(const ParticleSystemTemplate& t) : _worldBBox(BBox3::BoundBox), _particleCount(0),
	_autoDeleteEmitters(true)
{
	_emitterCount = t.getEmitterCount();
	if (_emitterCount > 0)
	{
		_emitters = new ParticleEmitter*[_emitterCount];
		for (unsigned int i = 0; i < t.getEmitterCount(); ++i)
			_emitters[i] = ParticleEmitterFactory::createParticleEmitter(*t.getEmitter(i));
	}
	else
		_emitters = 0;
}

ParticleSystem::~ParticleSystem()
{
	if (_autoDeleteEmitters)
	{
		for (int i = 0; i < _emitterCount; ++i)
			delete _emitters[i];

		if (_emitters)			delete[] _emitters;
	}
}

void ParticleSystem::setWorldMatrix(const Matrix4& w)
{
	for(int i=0;i<_emitterCount;++i)
		_emitters[i]->setWorldMatrix(w);
}

void ParticleSystem::update(float time, const Camera& c)
{
	_worldBBox=BBox3::InitBox;
	_particleCount=0;

	for(int i=0;i<_emitterCount;++i)
	{
		_emitters[i]->update(time,c);
		_worldBBox << _emitters[i]->getWorldBBox();
		_particleCount+=_emitters[i]->getParticleCount();
	}
}

void ParticleSystem::collectParticles(Particle** p, unsigned int& count, unsigned int max_count)
{
	for(int i=0;i<_emitterCount;++i)
		_emitters[i]->collectParticles(p,count,max_count);
}

void ParticleSystem::countParticles()
{
	_particleCount = 0;

	for (int i = 0; i<_emitterCount; ++i)
		_particleCount += _emitters[i]->getParticleCount();
}