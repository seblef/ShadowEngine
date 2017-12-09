
#include "stdafx.h"
#include "GameEntityTrigger.h"

const float EntityRotationSpeed = 90.0f;

extern ofstream	g_MediaOut;

GameEntityTrigger::GameEntityTrigger(EntityTrigger& t, const Matrix4& world) : GameEntity(t, world, t.isAnimated(), TRIGGER), _rotation(0)
{
	_physic = Physic::getSingletonRef().createTrigger(*t.getPhysicGeometry(), world);
	_physic->setUserData((GameObject*)this);
}

void GameEntityTrigger::update(float time)
{
	_rotation += time * EntityRotationSpeed;
	Matrix4 w;
	w.createRotateY(_rotation * M_PI / 180.0f);
	w(3, 0) = _world(3, 0);
	w(3, 1) = _world(3, 1);
	w(3, 2) = _world(3, 2);

	if (_rMesh)
		_rMesh->setWorldMatrix(w);
}

void GameEntityTrigger::onTrigger(GameCharacter* actor)
{
	Trigger *t = ((EntityTrigger*)_template)->getTrigger();
	if (t)
		t->apply(actor, this);
}