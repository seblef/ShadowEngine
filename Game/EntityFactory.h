
#ifndef _ENTITYFACTORY_H_
#define _ENTITYFACTORY_H_

#include "EntityTemplate.h"

class EntityFactory
{
public:

	EntityFactory()				{}

	static EntityTemplate*		loadEntity(const string& className, ScriptFile& sf);
};

#endif