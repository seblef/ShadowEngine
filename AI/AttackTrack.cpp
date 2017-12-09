
#include "stdafx.h"
#include "AttackTrack.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AISystem.h"
#include "AIDebugger.h"
#include <NavigationLib.h>

extern ofstream g_MediaOut;

#define ATTACK_MIN_DIST				3.0f
#define ATTACK_MAX_DIST				6.0f
#define ATTACK_DESIRED_DIST			4.5f

#define ATTACK_MIN_COSANGLE_FIRE	0.866f

bool AttackTrack::onEvent(AIAgent* a, const AIEvent& e)
{
	Vector3 dir;
	float dist;
	const Vector3& pos(a->getCallback()->getAgentPosition());
	const Vector3& playerPos(AISystem::getSingletonRef().getPlayerPos());

	switch (e.getName())
	{
	case AIEVT_ENTER:
	case AIEVT_UPDATE:
#ifdef _DEBUG_AI
		//		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving ENTER or UPDATE event.", a->getID(), getStateName());
#endif
		dir = playerPos - pos;
		dist = dir.getLength();
		dir/=dist;

		int destx, desty;

		if (dist <= ATTACK_MIN_DIST || dist >= ATTACK_MAX_DIST)
		{
			destx = (int)(playerPos.x - dir.x * ATTACK_DESIRED_DIST);
			desty = (int)(playerPos.z - dir.z * ATTACK_DESIRED_DIST);

			if (Navigation::getSingletonRef().getMap().findBestPlace(destx, desty, dir.x, dir.z, destx, desty))
				a->getCallback()->runTo(Vector3((float)destx, 0, (float)desty), dir);
		}
		else
			a->getCallback()->orientTo(dir);

		a->setLastKnownPlayerPosition(playerPos);
		tryFire(a, playerPos, dir, dist);

		return true;
		break;

	case AIEVT_LOSTPLAYER:
#ifdef _DEBUG_AI
		AIDebugger::getSingletonRef().logOut("Agent %i (%s) recieving LOSTPLAYER event.", a->getID(), getStateName());
#endif
		_machine->setState(a, ATTACK_SEARCH_STATE_ID);
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

void AttackTrack::tryFire(AIAgent* a, const Vector3& playerPos,
	const Vector3& agentToPlayerDir, float distToPlayer)
{
	IAICallback *callback = a->getCallback();
	if (callback->canFire() && distToPlayer < callback->getFireRange())
	{
		if ((callback->getAgentDirection() * agentToPlayerDir) >= ATTACK_MIN_COSANGLE_FIRE)
		{
#ifdef _DEBUG_AI
			AIDebugger::getSingletonRef().logOut("Agent %i firing.", a->getID());
#endif
			callback->fire();
		}
	}
}