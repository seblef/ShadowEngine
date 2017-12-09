
#ifndef _AIMACHINE_H_
#define _AIMACHINE_H_

#include "AIMachineState.h"
#include <map>

using namespace std;

class AIMachine
{
protected:

	typedef map<unsigned int, AIMachineState*>		StateMap;
	StateMap					_states;

	unsigned int				_id;

public:

	AIMachine(unsigned int id) : _id(id)			{}
	virtual ~AIMachine();

	AIMachineState*				getState(unsigned int stateID) const;
	void						setState(AIAgent *agent, unsigned int stateID) const;
	void						setState(AIAgent* agent, AIMachineState* state) const;

	virtual const char*			getMachineName() const = 0;
	virtual unsigned int		getStateID(const string& state) = 0;
	virtual bool				onGlobalEvent(AIAgent* agent, const AIEvent& e)		{ return false;  };
};

#endif