
#ifndef _ATTACKIDLE_H_
#define _ATTACKIDLE_H_

#include "AIMachineState.h"

class AttackIdle : public AIMachineState
{
public:

	AttackIdle(AIMachine* m) : AIMachineState(m)			{}

	bool						onEvent(AIAgent* a, const AIEvent& e);

	const char*					getStateName() const						{ return "idle"; }

};

#endif