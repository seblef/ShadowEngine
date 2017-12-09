
#include "stdafx.h"
#include "Particles.h"
#include "Renderer.h"

Particles::Particles(ParticleSystem* sys, const Matrix4& world, bool alwaysVisible) :
	Renderable(sys->getWorldBBox(), world, alwaysVisible), _system(sys)
{
	sys->setWorldMatrix(world);
}

Particles::~Particles()
{
	delete _system;
}

void Particles::setWorldMatrix(const Matrix4& world)
{
	_worldMatrix = world;
	_system->setWorldMatrix(world);
}

void Particles::enqueue()
{
	ParticleEngine::getSingletonRef().enqueueSystem(_system);
}

void Particles::update(float time)
{
	_system->update(time, *Renderer::getSingletonRef().getCurrentCamera());
	_worldBBox = _system->getWorldBBox();

	if (!_alwaysVisible)
		Renderer::getSingletonRef().getVisibilitySystem()->onRenderableMoved(this);
}