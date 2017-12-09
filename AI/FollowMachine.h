#ifndef _FOLLOWMACHINE_H_
#define _FOLLOWMACHINE_H_

#include "AIMachine.h"
#include "Ids.h"

class FollowMachine : public AIMachine
{
public:

	FollowMachine();
	~FollowMachine()				{}

	const char*					getMachineName() const		{ return "follow"; }
	unsigned int				getStateID(const string& name)
	{
		if (name == "idle")
			return FOLLOW_IDLE_STATE_ID;
		else if (name == "track")
			return FOLLOW_TRACK_STATE_ID;
		else if (name == "search")
			return FOLLOW_SEARCH_STATE_ID;
		else
			return 0xffffffff;
	}
};

#endif