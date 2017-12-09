
#include "stdafx.h"
#include "TriggerWeapon.h"
#include "GameSystem.h"
#include "GameEntityTrigger.h"
#include "Map.h"
#include "ActionServer.h"
#include "AddRemObject.h"

TriggerWeapon::TriggerWeapon(ScriptFile& sf) : _noRebirth(false)
{
	string t(sf.getToken());
	while (sf.good() && t != "end_trigger")
	{
		if (t == "weapon")				_weapon = sf.getToken();
		else if (t == "no_rebirth")		_noRebirth = true;

		t = sf.getToken();
	}
}

void TriggerWeapon::apply(GameCharacter* actor, GameEntityTrigger* entity)
{
	Weapon *w = GameSystem::getSingletonRef().getWeaponDB().getData(_weapon);
	assert(w);
	actor->setWeapon(w);

	GameSystem::getSingletonRef().getMap().remObject(entity);
	if (_noRebirth)
		delete entity;
	else
		ActionServer::getSingletonRef().addDelayedAction(new AddRemObject(entity),TRIGGER_REBIRTH_DELAY);
}