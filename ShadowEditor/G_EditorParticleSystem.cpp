
#include "stdafx.h"
#include "G_EditorParticleSystem.h"

#define EDITOR_MAX_PARTICLE_EMITTERS				256

G_EditorParticleSystem::G_EditorParticleSystem(const ParticleSystemTemplate& t)
{
	_emitters = new ParticleEmitter*[EDITOR_MAX_PARTICLE_EMITTERS];
	_emitterCount = t.getEmitterCount();

	for (int i = 0; i < _emitterCount; ++i)
		_emitters[i] = ParticleEmitterFactory::createParticleEmitter(*t.getEmitter(i));

	_system = new ParticleSystem(_emitterCount, _emitters, false);
}

G_EditorParticleSystem::~G_EditorParticleSystem()
{
	delete _system;
	for (int i = 0; i < _emitterCount; ++i)
		delete _emitters[i];
	delete[] _emitters;
}

void G_EditorParticleSystem::addEmitter(ParticleEmitter *e)
{
	if (_emitterCount < EDITOR_MAX_PARTICLE_EMITTERS)
	{
		_emitters[_emitterCount] = e;
		++_emitterCount;
		rebuildSystem();
	}
}

void G_EditorParticleSystem::remEmitter(int n)
{
	ParticleEmitter *old = _emitters[n];
	for (int i = n + 1; i < _emitterCount; ++i)
		_emitters[i - 1] = _emitters[i];

	--_emitterCount;
	rebuildSystem();
	delete old;
}

void G_EditorParticleSystem::rebuildSystem()
{
	ParticleSystem *s = new ParticleSystem(_emitterCount, _emitters, false);
	delete _system;
	_system = s;
}

ParticleSystemTemplate* G_EditorParticleSystem::buildTemplate()
{
	ParticleEmitterTemplate *et[EDITOR_MAX_PARTICLE_EMITTERS];
	ParticleEmitter* e;
	for (int i = 0; i < _emitterCount; ++i)
	{
		e = _emitters[i];
		et[i] = ParticleEmitterTemplateFactory::createTemplate(e->getEmitterName(),
			e->getRenderer()->getRendererName(), new ParticleMaterial(*e->getMaterial()),
			e->getParameters1(), e->getParameters2(), e->getMaxParticles(),
			e->getGravity(), e->getEmissionRate());
	}

	return new ParticleSystemTemplate(_emitterCount,et);
}