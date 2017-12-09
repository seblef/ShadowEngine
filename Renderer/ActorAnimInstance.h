
#ifndef _ACTORANIMINSTANCE_H_
#define _ACTORANIMINSTANCE_H_

#include <Core.h>

using namespace Core;

#define AANIM_TRANSITIONTIME				0.1f
#define AANIM_TRANSITIONFRAMERATE			20.0f

class ActorAnimInstance
{
protected:

	enum State
	{
		STATE_NOANIM=0,
		STATE_TRANSITION,
		STATE_ANIM,

		STATE_COUNT
	};

	bool					_loop, _reverse;
	float					_time;
	TTab<AnimPRSKeyVal>		_pose;
	State					_state;
	AnimPRSMultiTrack		_transitionTrack;
	const AnimPRSMultiTrack*	_track;

	void					buildTransitionTrack(const MatrixTab& pose, const AnimPRSMultiTrack& track, bool reverse);

public:

	ActorAnimInstance(int count);

	void					play(const AnimPRSMultiTrack* track, const MatrixTab& pose, bool loop, bool reverse);
	void					stop()				{ _track=0; }

	void					update(float time, MatrixTab& pose);
};

#endif