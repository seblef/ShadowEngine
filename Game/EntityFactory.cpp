
#include "stdafx.h"
#include "EntityFactory.h"
#include "EntityExplosive.h"
#include "EntityTrigger.h"

EntityTemplate* EntityFactory::loadEntity(const string& className, ScriptFile& sf)
{
	EntityTemplate* et = 0;
	if (className == "dynamic")
		et = new EntityDynamic(sf);
	else if (className == "explosive")
		et = new EntityExplosive(sf);
	else if (className == "trigger")
		et = new EntityTrigger(sf);

	return et;
}