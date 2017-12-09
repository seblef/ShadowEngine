
#include "stdafx.h"
#include "PhysicEventProcessor.h"
#include "GameEntityDynamic.h"
#include "GameEntityTrigger.h"
#include "Ammo.h"
#include <PhysicLib.h>

extern ofstream g_MediaOut;

#define EXTACT_GOBJ(x)			((GameObject*))

void PhysicEventProcessor::processEvents()
{
	int ecount;
	const PhysicEvent* events = Physic::getSingletonRef().getEvents(ecount);
	IPhysicObject *a, *t;

	for (int i = 0; i < ecount; ++i)
	{
		const PhysicEvent& e(events[i]);
		a = e.getActor();
		t = e.getTarget();

		PhysicObjectType ta = a->getObjectType();
		PhysicObjectType tt = t ? t->getObjectType() : POBJ_COUNT;

		switch (e.getType())
		{
		case PhysicEvent::TRIGGER:
			((GameEntityTrigger*)t->getUserData())->onTrigger((GameCharacter*)a->getUserData());
			break;

		case PhysicEvent::CONTACT:
			if (ta == POBJ_AMMO)
				((Ammo*)a->getUserData())->onContact(t);
			break;

		case PhysicEvent::WAKEUP:
			((GameEntityDynamic*)a->getUserData())->wakeUp();
			break;

		case PhysicEvent::SLEEP:
			((GameEntityDynamic*)a->getUserData())->sleep();
			break;
		}
	}

	Physic::getSingletonRef().clearEvents();
}
