
#include "stdafx.h"
#include "WeaponInstance.h"
#include "GameCharacter.h"
#include "GameSystem.h"
#include "GameEntityDynamic.h"
#include "ActionServer.h"
#include "Flash.h"

const float WeaponFlashAscend = 0.02f;
const float WeaponFlashStable = 0.02f;
const float WeaponFlashDescend = 0.02f;

WeaponInstance::WeaponInstance(Weapon* w, GameCharacter* c) : _template(w), _character(c), _lastFire(0)
{
	w->load();
	_rMeshInstance=new MeshInstance(w->getMesh(),Matrix4::Identity,true);
	c->getActorInstance()->attachMeshToNullNode(_rMeshInstance,_T("right_hand"));
}

WeaponInstance::~WeaponInstance()
{
	_character->getActorInstance()->detachMeshToNullNode(_T("right_hand"));
	delete _rMeshInstance;
}

bool WeaponInstance::canFire() const
{
	double currentTime = GameSystem::getSingletonRef().getGameTime();
	return (currentTime - _lastFire) >= _template->getFireTime();
}

void WeaponInstance::hit(const Hit& h) const
{
	GameObject::ObjectType t = h._object->getType();
	switch (t)
	{
	case GameObject::ACTOR:
		((GameCharacter*)h._object)->onHit(h);
		break;

	case GameObject::ENTITY:
		if (((GameEntity*)h._object)->getEntityType() == GameEntity::DYNAMIC)
			((GameEntityDynamic*)h._object)->onHit(h);
		break;
	}
}

void WeaponInstance::fire(const Vector3& origin) const
{
	if (_template->getFireSound())
	{
		SoundVirtualSource* src = SoundSystem::getSingletonRef().play(_template->getFireSound(), 1, true);
		ISoundVirtualSource* s = src->getSource();
		s->setPosition(origin);
		s->setInRadius(50.0f);
	}

	if (_template->isFlashEnable())
	{
		ActionServer::getSingletonRef().addAction(new Flash(
			_template->getFlashColor(),
			WeaponFlashAscend, WeaponFlashStable, WeaponFlashDescend,
			_template->getFlashRange(),
			origin));
	}
}