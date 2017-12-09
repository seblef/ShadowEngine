
#ifndef _FOLLOWIDLE_H_
#define _FOLLOWIDLE_H_

#include "AIMachineState.h"

class FollowIdle : public AIMachineState
{
public:

	FollowIdle(AIMachine* m) : AIMachineState(m)			{}

	bool						onEvent(AIAgent* a, const AIEvent& e);

	const char*					getStateName() const						{ return "idle"; }

};

#endif