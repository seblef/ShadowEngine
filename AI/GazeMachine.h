
#ifndef _GAZEMACHINE_H_
#define _GAZEMACHINE_H_

#include "AIMachine.h"
#include "Ids.h"

class GazeMachine : public AIMachine
{
public:

	GazeMachine();
	~GazeMachine()				{}

	const char*					getMachineName() const		{ return "gaze"; }
	unsigned int				getStateID(const string& name)
	{
		if (name == "idle")
			return GAZE_IDLE_STATE_ID;
		else if (name == "track")
			return GAZE_TRACK_STATE_ID;
		else
			return 0xffffffff;
	}
};

#endif