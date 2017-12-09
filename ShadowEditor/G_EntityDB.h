
#ifndef _G_ENTITYDB_H_
#define _G_ENTITYDB_H_

#include "G_EntityTemplate.h"

class G_EntityDB : public TDataBase<G_EntityTemplate>
{
public:

	G_EntityDB(const string& entitiesFile);
	

};

#endif