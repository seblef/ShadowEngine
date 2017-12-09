
#include "stdafx.h"
#include "EffectFactory.h"
#include "ExplosionTemplate.h"

Effect* EffectFactory::createEffect(const string& effectName, ScriptFile& sf)
{
	if (effectName == "explosion")
		return new ExplosionTemplate(sf);
	else
		return 0;
}