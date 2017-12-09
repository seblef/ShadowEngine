
#include "stdafx.h"
#include "GameParticles.h"

GameParticles::GameParticles(const TemplateParticleSystem& t, const Matrix4& world, bool alwaysVisible) : GameObject(PARTICLES,world,
	BBox3::NullBox, 0)
{
	ParticleSystem *ps = new ParticleSystem(*t.getTemplate());
	ps->setWorldMatrix(world);
	_system = new Particles(ps, world, alwaysVisible);
}

GameParticles::~GameParticles()
{
	delete _system;
}

void GameParticles::onAddToScene()
{
	Renderer::getSingletonRef().addRenderable(_system);
	_system->wakeUp();
}

void GameParticles::onRemFromScene()
{
	_system->sleep();
	Renderer::getSingletonRef().remRenderable(_system);
}