
#ifndef _SOUNDLISTENER_H_
#define _SOUNDLISTENER_H_

#include <XAudio2.h>
#include <X3DAudio.h>
#include "SoundCone.h"
#include "SoundUtil.h"


class SoundListener : public ISoundListener
{
protected:

	X3DAUDIO_LISTENER			_listener;
	SoundCone*					_cone;

public:

	SoundListener() : _cone(0)
	{
		soundCopyVec(Vector3::NullVector, _listener.Position);
		soundCopyVec(Vector3::NullVector, _listener.Velocity);
		soundCopyVec(Vector3::ZAxisVector, _listener.OrientFront);
		soundCopyVec(Vector3::YAxisVector, _listener.OrientTop);
		_listener.pCone = 0;
	}
	~SoundListener()			{}

	void			setPosition(const Vector3& pos)				{ soundCopyVec(pos, _listener.Position);  }
	void			setVelocity(const Vector3& vel)				{ soundCopyVec(vel, _listener.Velocity); }
	void			setFrontAxis(const Vector3& f)				{ soundCopyVec(f, _listener.OrientFront); }
	void			setTopAxis(const Vector3& t)				{ soundCopyVec(t, _listener.OrientTop); }
	void			setCone(ISoundCone* c)
	{
		_cone = (SoundCone*)c;
		_listener.pCone = _cone->getCone();
	}

	const Vector3&	getPosition() const							{ return SOUND_RETURN_VEC_CONST(_listener.Position); }
	const Vector3&	getVelocity() const							{ return SOUND_RETURN_VEC_CONST(_listener.Velocity); }
	const Vector3&	getFrontAxis() const						{ return SOUND_RETURN_VEC_CONST(_listener.OrientFront); }
	const Vector3&	getTopAxis() const							{ return SOUND_RETURN_VEC_CONST(_listener.OrientTop); }
	ISoundCone*		getCone() const								{ return _cone; }

	const X3DAUDIO_LISTENER&	getListener() const				{ return _listener; }
};

#endif