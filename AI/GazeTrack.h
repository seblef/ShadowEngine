
#ifndef _GAZETRACK_H_
#define _GAZETRACK_H_

#include "AIMachineState.h"

class GazeTrack : public AIMachineState
{
public:

	GazeTrack(AIMachine* m) : AIMachineState(m)		{}
	~GazeTrack()									{}

	bool					onEvent(AIAgent* a, const AIEvent& e);

	const char*				getStateName() const	{ return "track"; }
};

#endif