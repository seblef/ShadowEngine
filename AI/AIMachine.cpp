
#include "stdafx.h"
#include "AIMachine.h"
#include "AIAgent.h"
#include "AIDebugger.h"


AIMachine::~AIMachine()
{
	StateMap::iterator s(_states.begin());
	for (; s != _states.end(); ++s)
		delete s->second;
}

AIMachineState* AIMachine::getState(unsigned int stateID) const
{
	StateMap::const_iterator s(_states.find(stateID));
	return s == _states.end() ? 0 : s->second;
}

void AIMachine::setState(AIAgent* agent, unsigned int stateID) const
{
	setState(agent, getState(stateID));
}

void AIMachine::setState(AIAgent* agent, AIMachineState* state) const
{
	assert(state && state->getMachine()==this);

#ifdef _DEBUG_AI
	AIDebugger::getSingletonRef().logOut("Agent %i is going from state %s to %s.",
		agent->getID(), agent->getCurrentState()->getStateName(), state->getStateName());
#endif

	agent->setNextState(state);
	agent->setForceChangeState(true);
}
