
#ifndef _ENTITYDB_H_
#define _ENTITYDB_H_

#include "EntityTemplate.h"

class EntityDB : public TDataBase<EntityTemplate>
{
public:

	EntityDB(const string& entityFile);

	void				unloadEntities();
};

#endif