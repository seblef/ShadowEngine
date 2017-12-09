
#include "stdafx.h"
#include "FollowIdle.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AIDebugger.h"

bool FollowIdle::onEvent(AIAgent* a, const AIEvent& e)
{
	switch (e.getName())
	{
	case AIEVT_ENTER:
#ifdef _DEBUG_AI
		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving ENTER event.", a->getID(), getStateName());
#endif
		a->getCallback()->idle();
		return true;
		break;

	case AIEVT_SEEPLAYER:
#ifdef _DEBUG_AI
		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving SEEPLAYER event.", a->getID(), getStateName());
#endif
		_machine->setState(a, FOLLOW_TRACK_STATE_ID);
		return true;
		break;

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