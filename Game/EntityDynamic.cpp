
#include "stdafx.h"
#include "EntityDynamic.h"
#include "GameEntityDynamic.h"

EntityDynamic::EntityDynamic() : EntityTemplate(false), _density(1.0f)
{
}

EntityDynamic::EntityDynamic(ScriptFile& sf) : EntityTemplate(false)
{
	string t(sf.getToken());
	while (sf.good() && t != "end_entity")
	{
		if (t == "density")
			_density = stof(sf.getToken());
		else
			EntityTemplate::parseToken(t, sf);

		t = sf.getToken();
	}
}

void EntityDynamic::load()
{
	EntityTemplate::load();
}

void EntityDynamic::unload()
{
	EntityTemplate::unload();
}

GameEntity* EntityDynamic::createInstance(const Matrix4& world)
{
	return new GameEntityDynamic(*this,world);
}

void EntityDynamic::parseToken(const string& token, ScriptFile& sf)
{
	if (token == "density")			_density = stof(sf.getToken());
	else							EntityTemplate::parseToken(token, sf);
}