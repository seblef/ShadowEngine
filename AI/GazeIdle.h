
#ifndef _GAZEIDLE_H_
#define _GAZEIDLE_H_

#include "AIMachineState.h"

class GazeIdle : public AIMachineState
{
public:

	GazeIdle(AIMachine* m) : AIMachineState(m)			{}

	bool						onEvent(AIAgent* a, const AIEvent& e);

	const char*					getStateName() const						{ return "idle"; }
};

#endif