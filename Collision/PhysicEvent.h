
#ifndef _PHYSICEVENT_H_
#define _PHYSICEVENT_H_

#include "IPhysicObject.h"

class PhysicEvent
{
public:

	enum EventType
	{
		TRIGGER=0,
		CONTACT,
		WAKEUP,
		SLEEP,

		COUNT
	};

protected:

	EventType				_type;

	IPhysicObject*			_actor;
	IPhysicObject*			_target;

public:

	PhysicEvent()			{}
	PhysicEvent(EventType t, IPhysicObject* actor, IPhysicObject* target) :
		_type(t), _actor(actor), _target(target)			{}
	PhysicEvent(const PhysicEvent& e) : _type(e._type), _actor(e._actor), _target(e._target)	{}

	PhysicEvent&			operator=(const PhysicEvent& e)
	{
		_type = e._type;
		_actor = e._actor;
		_target = e._target;
		return *this;
	}

	EventType				getType() const					{ return _type; }
	IPhysicObject*			getActor() const				{ return _actor; }
	IPhysicObject*			getTarget() const				{ return _target; }
};

#endif