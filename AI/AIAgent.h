
#ifndef _AIAGENT_H_
#define _AIAGENT_H_

#include "AIMachine.h"
#include "IAICallback.h"

class AIAgent
{
private:

	unsigned int				_id;
	IAICallback*				_callback;

	AIMachine*					_currentMachine;
	AIMachineState*				_currentState;
	AIMachineState*				_nextState;
	bool						_forceStateChange;

	Vector3						_lastKnownPlayerPosition;

	bool						_canDelete;

public:

	AIAgent(AIMachineState* currentState, AIMachine* currentMachine,
		IAICallback *callback, unsigned int id);
	~AIAgent()		{}

	unsigned int				getID() const						{ return _id; }

	AIMachine*					getCurrentMachine() const			{ return _currentMachine; }
	AIMachineState*				getCurrentState() const				{ return _currentState; }
	AIMachineState*				getNextState() const				{ return _nextState; }
	bool						getForceStateChange() const			{ return _forceStateChange; }
	bool						getCanDelete() const				{ return _canDelete; }
	const Vector3&				getLastKnownPlayerPosition() const	{ return _lastKnownPlayerPosition; }

	void						setMachine(AIMachine* m, AIMachineState* s);
	void						setCurrentState(AIMachineState* s)	{ _currentState = s; }
	void						setNextState(AIMachineState* s)		{ _nextState = s; }
	void						setForceChangeState(bool f)			{ _forceStateChange = f; }
	void						setCanDelete(bool d)				{ _canDelete = d; }
	void						setLastKnownPlayerPosition(const Vector3& p)		{ _lastKnownPlayerPosition = p; }

	IAICallback*				getCallback() const					{ return _callback; }
};

#endif