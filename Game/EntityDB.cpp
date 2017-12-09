
#include "stdafx.h"
#include "EntityDB.h"
#include "EntityFactory.h"

EntityDB::EntityDB(const string& entityFile)
{
	EntityTemplate* e;
	string className, entityName;

	ScriptFile sf(entityFile);
	assert(sf.good());

	while (sf.good())
	{
		if (sf.getToken() == "entity")
		{
			className = sf.getToken();
			entityName = sf.getToken();

			e = EntityFactory::loadEntity(className, sf);
			assert(e);
			registerData(entityName, e);
		}
	}
}

void EntityDB::unloadEntities()
{
	map<string, EntityTemplate*>::iterator t(_data.begin());
	for (; t != _data.end(); ++t)
		t->second->unload();
}