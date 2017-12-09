
#include "stdafx.h"
#include "AIAgent.h"
#include "AISystem.h"


AIAgent::AIAgent(AIMachineState* currentState, AIMachine* currentMachine,
	IAICallback *callback, unsigned int id) :
	_id(id), _currentMachine(currentMachine), _currentState(currentState), _nextState(0), _forceStateChange(false),
	_canDelete(false), _callback(callback)
{
	AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_ENTER, id, id));
}

void AIAgent::setMachine(AIMachine* m, AIMachineState* s)
{
	_currentState->onEvent(this,AIEvent(AIEVT_EXIT,_id,_id));
	_currentMachine = m;
	_currentState = s;
	_nextState = 0;
	_forceStateChange = 0;
	AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_ENTER, _id, _id));
}