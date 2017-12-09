
#include "stdafx.h"
#include "GazeTrack.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AISystem.h"
#include "AIDebugger.h"

bool GazeTrack::onEvent(AIAgent* a, const AIEvent& e)
{
	Vector3 dir;

	switch (e.getName())
	{
	case AIEVT_ENTER:
	case AIEVT_UPDATE:
#ifdef _DEBUG_AI
//		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving ENTER or UPDATE event.", a->getID(), getStateName());
#endif
		dir = AISystem::getSingletonRef().getPlayerPos() - a->getCallback()->getAgentPosition();
		dir.normalize();
		a->getCallback()->orientTo(dir);
		return true;

	case AIEVT_LOSTPLAYER:
#ifdef _DEBUG_AI
		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving LOSTPLAYER event.", a->getID(), getStateName());
#endif
		_machine->setState(a, GAZE_IDLE_STATE_ID);
		return true;
	}

	return false;
}