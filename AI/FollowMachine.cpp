
#include "stdafx.h"
#include "FollowMachine.h"
#include "FollowTrack.h"
#include "FollowIdle.h"
#include "FollowSearch.h"

FollowMachine::FollowMachine() : AIMachine(FOLLOW_MACHINE_ID)
{
	_states[FOLLOW_IDLE_STATE_ID] = new FollowIdle(this);
	_states[FOLLOW_TRACK_STATE_ID] = new FollowTrack(this);
	_states[FOLLOW_SEARCH_STATE_ID] = new FollowSearch(this);
}