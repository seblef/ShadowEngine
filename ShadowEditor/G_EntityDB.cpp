
#include "stdafx.h"
#include "G_EntityDB.h"

G_EntityDB::G_EntityDB(const string& file)
{
	ScriptFile sf(file);
	assert(sf.good());

	string t(sf.getToken());
	while (sf.good())
	{
		if (t == "entity")
		{
			G_EntityTemplate* e = new G_EntityTemplate(sf);
			registerData(e->getName(), e);
		}
		t = sf.getToken();
	}
}