
#include "stdafx.h"
#include "AttackSearch.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AISystem.h"
#include "AIDebugger.h"
#include <NavigationLib.h>


bool AttackSearch::onEvent(AIAgent* a, const AIEvent& e)
{
	Vector3 dir;
	float dist;
	const Vector3& pos(a->getCallback()->getAgentPosition());

	switch (e.getName())
	{
	case AIEVT_ENTER:
#ifdef _DEBUG_AI
		//		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving ENTER event.", a->getID(), getStateName());
#endif
		dir = a->getLastKnownPlayerPosition() - pos;
		dir.normalize();
		a->getCallback()->runTo(a->getLastKnownPlayerPosition(), dir);
		return true;
		break;

	case AIEVT_SEEPLAYER:
#ifdef _DEBUG_AI
		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving SEEPLAYER event.", a->getID(), getStateName());
#endif
		_machine->setState(a, ATTACK_TRACK_STATE_ID);
		return true;
		break;

	case AIEVT_ENDMOVE:
#ifdef _DEBUG_AI
		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving ENDMOVE event. Going to home position", a->getID(), getStateName());
#endif
		a->getCallback()->goHome();
		_machine->setState(a, ATTACK_IDLE_STATE_ID);
		return true;

	case AIEVT_HIT:
#ifdef _DEBUG_AI
		int damage = e.getIntParam();
		AIDebugger::getSingletonRef().logOut("Agent %i hitted by agent %i (damage: %f)",
			a->getID(), e.getSenderID(), *((float*)&damage));
#endif
		return true;
	}

	return false;
}