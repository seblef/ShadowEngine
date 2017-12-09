
#include "stdafx.h"
#include "G_Actor.h"
#include "G_Selection.h"

G_Actor::G_Actor(Character* c) : G_Object(ACTOR), _template(c)
{
	c->load();
	_localBox = c->getActor()->getBBox();
	_selItem = G_Selection::getSingletonRef().createBoxActor(_localBox.getSize());
	_selItem->setUserData((G_Object*)this);
}

G_Actor::G_Actor(const G_Actor& a) : G_Object(&a), _template(a._template)
{
	_localBox = _template->getActor()->getBBox();
	_selItem = G_Selection::getSingletonRef().createBoxActor(_localBox.getSize());
	_selItem->setUserData((G_Object*)this);
}

void G_Actor::rebuildMatrix()
{
	G_Object::rebuildMatrix();

	if (_rActor)
	{
		_rActor->setWorldMatrix(_world);
		_rActor->update(0.0f);
	}
}

void G_Actor::onAddToScene()
{
	_rActor = new ActorInstance(_template->getActor(), _world, false);

	Renderer::getSingletonRef().addRenderable(_rActor);
	G_Object::onAddToScene();
}

void G_Actor::onRemFromScene()
{
	Renderer::getSingletonRef().remRenderable(_rActor);
	delete _rActor;
	_rActor = 0;

	G_Object::onRemFromScene();
}