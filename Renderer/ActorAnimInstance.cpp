
#include "stdafx.h"
#include "ActorAnimInstance.h"

ActorAnimInstance::ActorAnimInstance(int count) : _time(0), _pose(count), _track(0), _loop(false), _reverse(false),
	_transitionTrack((int)(AANIM_TRANSITIONFRAMERATE * AANIM_TRANSITIONTIME) + 1,count), _state(STATE_NOANIM)
{
	float step=1.0f / AANIM_TRANSITIONFRAMERATE;
	float t=0;
	for(int i=0;i<_transitionTrack.getKeyCount();++i,t+=step)
		_transitionTrack.setTime(i,t);
}

void ActorAnimInstance::play(const AnimPRSMultiTrack* track, const MatrixTab& pose, bool loop, bool reverse)
{
	_state=STATE_TRANSITION;
	buildTransitionTrack(pose,*track,reverse);

	_time=0;
	_loop=loop;
	_reverse=reverse;
	_track=track;
}

void ActorAnimInstance::update(float time, MatrixTab& pose)
{
	_time+=time;

	const AnimPRSMultiTrack *t=0;
	if(_state==STATE_TRANSITION)
	{
		if(_time >= AANIM_TRANSITIONTIME)
		{
			_state=STATE_ANIM;
			_time-=AANIM_TRANSITIONTIME;
			t=_track;
		}
		else
			t=&_transitionTrack;
	}
	else if(_state==STATE_ANIM)
		t=_track;

	if(t)
	{
		if(_time >= t->getTrackTime())
			_time-=t->getTrackTime();

		t->evaluate(_time,_pose,_loop,_reverse);
		for(int i=0;i<_pose.getCount();++i)
			_pose.getItem(i).getMatrix4(pose.getItem(i));
	}
}

void ActorAnimInstance::buildTransitionTrack(const MatrixTab& pose, const AnimPRSMultiTrack& track, bool reverse)
{
	Quaternion qstart,qend,qnow;
	Vector3 vstart,vend,vnow;
	Matrix3 rotScale;
	float t;
	float dt=1.0f / (float)(_transitionTrack.getKeyCount()-1);
	int end_index=reverse ? track.getKeyCount()-1 : 0;

	for(int i=0;i<track.getTrackCount();++i)
	{
		qstart.buildMatrix(pose.getItem(i));
		qend.buildMatrix(track.getValue(end_index,i).getRotationScale());

		vstart=pose.getItem(i);
		vend=track.getValue(end_index,i).getPosition();

		t=0;
		for(int j=0;j<_transitionTrack.getKeyCount();++j,t+=dt)
		{
			Quaternion::slerp(qstart,qend,t,qnow);
			Vector3::slerp(vstart,vend,t,vnow);

			qnow.getMatrix(rotScale);

			_transitionTrack.getValue(j,i).setPosition(vnow);
			_transitionTrack.getValue(j,i).setRotationScale(rotScale);
		}
	}
}