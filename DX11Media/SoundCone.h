
#ifndef _SOUNDCONE_H_
#define _SOUNDCONE_H_

#include <MediaCommon.h>
#include <X3DAudio.h>

class SoundCone : public ISoundCone
{
protected:

	X3DAUDIO_CONE		_cone;

public:

	SoundCone()
	{
		_cone.InnerAngle = 0;				_cone.OuterAngle = 0;
		_cone.InnerVolume = 0;				_cone.OuterVolume = 1;
		_cone.InnerLPF = 0;					_cone.OuterLPF = 1;
		_cone.InnerReverb = 0;				_cone.OuterReverb = 1;
	}
	SoundCone(float iAngle, float oAngle, float iVolume, float oVolume,
		float iLPF, float oLPF, float iReverb, float oReverb)
	{
		_cone.InnerAngle = iAngle;			_cone.OuterAngle = oAngle;
		_cone.InnerVolume = iVolume;		_cone.OuterVolume = oVolume;
		_cone.InnerLPF = iLPF;				_cone.OuterLPF = oLPF;
		_cone.InnerReverb = iReverb;		_cone.OuterReverb = oReverb;
	}

	void				setInnerAngle(float a)				{ _cone.InnerAngle = a; }
	void				setOuterAngle(float a)				{ _cone.OuterAngle = a; }
	void				setInnerVolume(float v)				{ _cone.InnerVolume = v; }
	void				setOuterVolume(float v)				{ _cone.OuterVolume = v; }
	void				setInnerLPF(float lpf)				{ _cone.InnerLPF = lpf; }
	void				setOuterLPF(float lpf)				{ _cone.OuterLPF = lpf; }
	void				setInnerReverb(float r)				{ _cone.InnerReverb = r; }
	void				setOuterReverb(float r)				{ _cone.OuterReverb = r; }

	float				getInnerAngle()						{ return _cone.InnerAngle; }
	float				getOuterAngle()						{ return _cone.OuterAngle; }
	float				getInnerVolume()					{ return _cone.InnerVolume; }
	float				getOuterVolume()					{ return _cone.OuterVolume; }
	float				getInnerLPF()						{ return _cone.InnerLPF; }
	float				getOuterLPF()						{ return _cone.OuterLPF; }
	float				getInnerReverb()					{ return _cone.InnerReverb; }
	float				getOuterReverb()					{ return _cone.OuterReverb; }

	X3DAUDIO_CONE*		getCone()							{ return &_cone; }
};

#endif