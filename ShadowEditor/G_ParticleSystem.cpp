
#include "stdafx.h"
#include "G_ParticleSystem.h"
#include "G_Selection.h"
#include "G_EditorRenderer.h"

G_ParticleSystem::G_ParticleSystem(G_ParticlesTemplate* t) : G_Object(PARTICLESYSTEM), _template(t)
{
	_selItem = G_Selection::getSingletonRef().createSphereActor(0.25f);
	_selItem->setUserData((G_Object*)this);
}

G_ParticleSystem::G_ParticleSystem(const G_ParticleSystem& ps) : G_Object(PARTICLESYSTEM), _template(ps._template)
{
	_selItem = G_Selection::getSingletonRef().createSphereActor(0.25f);
	_selItem->setUserData((G_Object*)this);
}

void G_ParticleSystem::onAddToScene()
{
	G_Object::onAddToScene();
	G_EditorRenderer::getSingletonRef().addObject(this);
}

void G_ParticleSystem::onRemFromScene()
{
	G_Object::onRemFromScene();
	G_EditorRenderer::getSingletonRef().remObject(this);
}

void G_ParticleSystem::drawSelection(G_EditorObjDrawer *d)
{
	float r = 1.0f;

	d->setColor(Color::Red);
	d->setWorldMatrix(_world);
	d->drawCircle(Vector3::NullVector, Vector3::XAxisVector, r);
	d->drawCircle(Vector3::NullVector, Vector3::YAxisVector, r);
	d->drawCircle(Vector3::NullVector, Vector3::ZAxisVector, r);
}