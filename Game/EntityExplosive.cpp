
#include "stdafx.h"
#include "EntityExplosive.h"
#include "ExplosionTemplate.h"
#include "GameEntityExplosive.h"
#include "GameSystem.h"

EntityExplosive::EntityExplosive(ScriptFile& sf) : _damageThreshold(20.0f)
{
	string t(sf.getToken());
	while (sf.good() && t != "end_entity")
	{
		if (t == "explosion")
			_explosionName = sf.getToken();
		else if (t == "damage_threshold")
			_damageThreshold = stof(sf.getToken());
		else
			EntityDynamic::parseToken(t, sf);

		t = sf.getToken();
	}
}

EntityExplosive::~EntityExplosive()
{
}

void EntityExplosive::load()
{
	EntityDynamic::load();

	_explosionTemplate = (ExplosionTemplate*)GameSystem::getSingletonRef().getEffectDB().getData(_explosionName);
	assert(_explosionTemplate);

	_explosionTemplate->load();
}

GameEntity* EntityExplosive::createInstance(const Matrix4& world)
{
	return new GameEntityExplosive(*this,world);
}

