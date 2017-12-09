
#ifndef _AIMACHINESTATE_H_
#define _AIMACHINESTATE_H_

#include "AIEvent.h"

class AIAgent;
class AIMachine;

class AIMachineState
{
protected:

	AIMachine*				_machine;

public:

	AIMachineState(AIMachine* machine)	: _machine(machine)		{}
	virtual ~AIMachineState()	{}

	AIMachine*				getMachine() const			{ return _machine; }

	virtual bool			onEvent(AIAgent *agent, const AIEvent& event) = 0;
	virtual const char*		getStateName() const = 0;
};

#endif