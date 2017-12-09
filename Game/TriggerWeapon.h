
#ifndef _TRIGGERWEAPON_H_
#define _TRIGGERWEAPON_H_

#include "Trigger.h"

class TriggerWeapon : public Trigger
{
protected:

	string				_weapon;
	bool				_noRebirth;

public:

	TriggerWeapon(ScriptFile& sf);

	void				apply(GameCharacter* actor, GameEntityTrigger* entity);
};

#endif