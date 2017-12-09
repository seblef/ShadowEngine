
#ifndef _EFFECTFACTORY_H_
#define _EFFECTFACTORY_H_

#include "Effect.h"
#include <Core.h>

using namespace Core;

class EffectFactory
{
public:

	EffectFactory()			{}

	static Effect*					createEffect(const string& effectName, ScriptFile& sf);
};

#endif