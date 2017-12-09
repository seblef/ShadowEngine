
#include "stdafx.h"
#include "ActionServer.h"

ActionServer::~ActionServer()
{
	reset();
}

void ActionServer::reset()
{
	ActionSet::iterator a(_activeActions.begin());
	for (; a != _activeActions.end(); ++a)
		delete *a;

	_activeActions.clear();
}

void ActionServer::addDelayedAction(Action *a, float delay)
{
	assert(delay > 0.0f);
	a->getDelayTime() = delay;
	_delayedActions.insert(a);
}

void ActionServer::update(float time)
{
	Action *action;

	ActionSet::iterator a(_delayedActions.begin());
	for (; a != _delayedActions.end(); ++a)
	{
		action = *a;
		action->getDelayTime() -= time;
		if (action->getDelayTime() < 0.0f)
		{
			_activeActions.insert(action);
			_deadActions.push_back(action);
		}
	}

	ActionVector::iterator da(_deadActions.begin());
	for (; da != _deadActions.end(); ++da)
		_delayedActions.erase(*da);

	_deadActions.clear();

	for (a=_activeActions.begin(); a != _activeActions.end(); ++a)
	{
		action = *a;
		action->update(time);
		if (action->isDead())
			_deadActions.push_back(action);
	}

	for (da=_deadActions.begin(); da != _deadActions.end(); ++da)
	{
		_activeActions.erase(*da);
		delete *da;
	}

	_deadActions.clear();
}