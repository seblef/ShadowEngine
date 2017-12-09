
#include "stdafx.h"
#include "GazeIdle.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AIDebugger.h"

bool GazeIdle::onEvent(AIAgent* a, const AIEvent& e)
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
		_machine->setState(a, GAZE_TRACK_STATE_ID);
		return true;
		break;
	}

	return false;
}