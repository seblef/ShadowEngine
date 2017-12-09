
#include "stdafx.h"
#include "G_Sound.h"
#include "G_Selection.h"
#include "G_EditorRenderer.h"

G_Sound::G_Sound(G_SoundTemplate* t) : G_Object(SOUND), _template(t)
{
	_selItem = G_Selection::getSingletonRef().createSphereActor(0.25f);
	_selItem->setUserData((G_Object*)this);
}

G_Sound::G_Sound(const G_Sound& ps) : G_Object(PARTICLESYSTEM), _template(ps._template)
{
	_selItem = G_Selection::getSingletonRef().createSphereActor(0.25f);
	_selItem->setUserData((G_Object*)this);
}

void G_Sound::onAddToScene()
{
	G_Object::onAddToScene();
	G_EditorRenderer::getSingletonRef().addObject(this);
}

void G_Sound::onRemFromScene()
{
	G_Object::onRemFromScene();
	G_EditorRenderer::getSingletonRef().remObject(this);
}

void G_Sound::drawSelection(G_EditorObjDrawer *d)
{
	float r = _template->getRadius();

	d->setColor(Color::Red);
	d->setWorldMatrix(_world);
	d->drawCircle(Vector3::NullVector, Vector3::XAxisVector, r);
	d->drawCircle(Vector3::NullVector, Vector3::YAxisVector, r);
	d->drawCircle(Vector3::NullVector, Vector3::ZAxisVector, r);
}