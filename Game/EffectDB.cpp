
#include "stdafx.h"
#include "EffectDB.h"
#include "EffectFactory.h"


EffectDB::EffectDB(const string& effectsFile)
{
	ScriptFile sf(effectsFile);
	assert(sf.good());

	string t(sf.getToken());
	string className;
	string effectName;

	while (sf.good())
	{
		if (t == "effect")
		{
			className = sf.getToken();
			effectName = sf.getToken();

			registerData(effectName, EffectFactory::createEffect(className, sf));
		}
		t = sf.getToken();
	}
}

void EffectDB::unloadEffects() const
{
	map<string, Effect*>::const_iterator e(_data.begin());
	for (; e != _data.end(); ++e)
		e->second->unload();
}