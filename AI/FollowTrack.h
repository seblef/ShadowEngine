
#ifndef _FOLLOWTRACK_H_
#define _FOLLOWTRACK_H_

#include "AIMachineState.h"

class FollowTrack : public AIMachineState
{
public:

	FollowTrack(AIMachine* m) : AIMachineState(m)		{}

	bool					onEvent(AIAgent* a, const AIEvent& e);

	const char*				getStateName() const	{ return "track"; }
};

#endif