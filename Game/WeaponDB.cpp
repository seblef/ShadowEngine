
#include "stdafx.h"
#include "WeaponDB.h"
#include "WeaponFactory.h"

WeaponDB::WeaponDB(const string& weaponFile)
{
	Weapon *w;
	string className,weaponName;
	ScriptFile sf(weaponFile);
	assert(sf.good());

	while(sf.good())
	{
		if(sf.getToken()==_T("weapon"))
		{
			className=sf.getToken();
			weaponName=sf.getToken();

			w=WeaponFactory::loadWeapon(className,sf);
			registerData(weaponName,w);
		}
	}
}

void WeaponDB::unloadWeapons()
{
	map<string,Weapon*>::iterator w(_data.begin());
	for(;w!=_data.end();++w)
		w->second->unload();
}