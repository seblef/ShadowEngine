
#include "stdafx.h"
#include "Trigger.h"
#include "TriggerWeapon.h"

Trigger* TriggerFactory::createTrigger(const string& className, ScriptFile& sf)
{
	if (className == "weapon")
		return new TriggerWeapon(sf);
	else
		return 0;
}