
#ifndef _ACTIONSERVER_H_
#define _ACTIONSERVER_H_

#include "Action.h"
#include <Core.h>

using namespace Core;
using namespace std;

class ActionServer : public TSingleton<ActionServer>
{
protected:

	typedef set<Action*>	ActionSet;
	typedef vector<Action*>	ActionVector;
	
	ActionSet				_activeActions;
	ActionSet				_delayedActions;
	ActionVector			_deadActions;

public:

	ActionServer()			{}
	~ActionServer();

	void					reset();

	void					addAction(Action* a)			{ _activeActions.insert(a); }
	void					addDelayedAction(Action *a, float delay);
	void					update(float time);
};

#endif