
#include "stdafx.h"
#include "GazeMachine.h"
#include "GazeTrack.h"
#include "GazeIdle.h"

GazeMachine::GazeMachine() : AIMachine(GAZE_MACHINE_ID)
{
	_states[GAZE_IDLE_STATE_ID] = new GazeIdle(this);
	_states[GAZE_TRACK_STATE_ID] = new GazeTrack(this);
}