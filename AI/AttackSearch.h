
#ifndef _ATTACKSEARCH_H_
#define _ATTACKSEARCH_H_

#include "AIMachineState.h"

class AttackSearch : public AIMachineState
{
public:

	AttackSearch(AIMachine* m) : AIMachineState(m)		{}

	bool					onEvent(AIAgent* a, const AIEvent& e);

	const char*				getStateName() const	{ return "search"; }

};

#endif