
#include "stdafx.h"
#include "CharacterDB.h"

CharacterDB::CharacterDB(const string& playersFile)
{
	Character *p;

	ScriptFile sf(playersFile);
	while(sf.good())
	{
		if(sf.getToken()=="character")
		{
			p=new Character(sf);
			registerData(p->getName(),p);
		}
	}
}

void CharacterDB::unloadCharacters()
{
	map<string,Character*>::iterator c(_data.begin());
	for(;c!=_data.end();++c)
		c->second->unload();
}