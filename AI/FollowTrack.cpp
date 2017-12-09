
#include "stdafx.h"
#include "FollowTrack.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AISystem.h"
#include "AIDebugger.h"
#include <NavigationLib.h>

extern ofstream g_MediaOut;

#define FOLLOW_MIN_DIST				1.0f
#define FOLLOW_MAX_DIST				4.0f
#define FOLLOW_DESIRED_DIST			2.5f

bool FollowTrack::onEvent(AIAgent* a, const AIEvent& e)
{
	Vector3 dir;
	Vector3 playerPos;
	float dist;
	const Vector3& pos(a->getCallback()->getAgentPosition());

	switch (e.getName())
	{
	case AIEVT_ENTER:
	case AIEVT_UPDATE:
#ifdef _DEBUG_AI
		//		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving ENTER or UPDATE event.", a->getID(), getStateName());
#endif
		playerPos = AISystem::getSingletonRef().getPlayerPos();
		dir = playerPos - pos;
		dist = dir.getLength();
		dir.normalize();

		int destx, desty;

		if (dist <= FOLLOW_MIN_DIST || dist >= FOLLOW_MAX_DIST)
		{
			destx = (int)(playerPos.x - dir.x * FOLLOW_DESIRED_DIST);
			desty = (int)(playerPos.z - dir.z * FOLLOW_DESIRED_DIST);

			if (Navigation::getSingletonRef().getMap().findBestPlace(destx, desty, dir.x, dir.z, destx, desty))
				a->getCallback()->runTo(Vector3((float)destx, 0, (float)desty), dir);
		}
		else
			a->getCallback()->orientTo(dir);

		a->setLastKnownPlayerPosition(AISystem::getSingletonRef().getPlayerPos());

		return true;
		break;

	case AIEVT_LOSTPLAYER:
#ifdef _DEBUG_AI
		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving LOSTPLAYER event.", a->getID(), getStateName());
#endif
		_machine->setState(a, FOLLOW_SEARCH_STATE_ID);
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