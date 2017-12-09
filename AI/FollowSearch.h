
#ifndef _FOLLOWSEARCH_H_
#define _FOLLOWSEARCH_H_

#include "AIMachineState.h"

class FollowSearch : public AIMachineState
{
public:

	FollowSearch(AIMachine* m) : AIMachineState(m)		{}

	bool					onEvent(AIAgent* a, const AIEvent& e);

	const char*				getStateName() const	{ return "search"; }

};

#endif