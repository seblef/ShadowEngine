#ifndef _ATTACKMACHINE_H_
#define _ATTACKMACHINE_H_

#include "AIMachine.h"
#include "Ids.h"

class AttackMachine : public AIMachine
{
public:

	AttackMachine();
	~AttackMachine()				{}

	const char*					getMachineName() const		{ return "attack"; }
	unsigned int				getStateID(const string& name)
	{
		if (name == "idle")
			return ATTACK_IDLE_STATE_ID;
		else if (name == "track")
			return ATTACK_TRACK_STATE_ID;
		else if (name == "search")
			return ATTACK_SEARCH_STATE_ID;
		else
			return 0xffffffff;
	}
};

#endif