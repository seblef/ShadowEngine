
#include "stdafx.h"
#include "EntityTrigger.h"
#include "GameEntityTrigger.h"

EntityTrigger::EntityTrigger(ScriptFile& sf) : EntityTemplate(true), _trigger(0), _animated(false)
{
	string t(sf.getToken());
	while (sf.good() && t != "end_entity")
	{
		if (t == "trigger")			_trigger = TriggerFactory::createTrigger(sf.getToken(), sf);
		else if (t == "animated")	_animated = true;
		else
			parseToken(t, sf);

		t = sf.getToken();
	}
}

EntityTrigger::~EntityTrigger()
{
	if (_trigger)			delete _trigger;
}

void EntityTrigger::load()
{
	EntityTemplate::load();
	if (_trigger)		_trigger->load();
}

void EntityTrigger::unload()
{
	EntityTemplate::unload();
	if (_trigger)		_trigger->unload();
}

GameEntity* EntityTrigger::createInstance(const Matrix4& world)
{
	return new GameEntityTrigger(*this, world);
}