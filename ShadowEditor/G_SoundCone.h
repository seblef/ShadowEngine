
#ifndef _G_SOUNDCONE_H_
#define _G_SOUNDCONE_H_

class G_SoundCone
{
protected:

	float				_inAngle, _outAngle;
	float				_inVolume, _outVolume;
	float				_inLPF, _outLPF;
	float				_inReverb, _outReverb;

public:

	G_SoundCone() : _inAngle(0), _outAngle(0), _inVolume(0), _outVolume(1), _inLPF(0), _outLPF(1), _inReverb(0), _outReverb(1)		{}
	G_SoundCone(float iAngle, float oAngle, float iVol, float oVol, float iLPF, float oLPF, float iRev, float oRev) :
		_inAngle(iAngle), _outAngle(oAngle), _inVolume(iVol), _outVolume(oVol), _inLPF(iLPF), _outLPF(oLPF),
		_inReverb(iRev), _outReverb(oRev)																							{}
	G_SoundCone(const G_SoundCone& c) : _inAngle(c._inAngle), _outAngle(c._outAngle), _inVolume(c._inVolume), _outVolume(c._outVolume),
		_inLPF(c._inLPF), _outLPF(c._outLPF), _inReverb(c._inReverb), _outReverb(c._outReverb)										{}

	float				getInAngle() const				{ return _inAngle; }
	float				getOutAngle() const				{ return _outAngle; }
	float				getInVolume() const				{ return _inVolume; }
	float				getOutVolume() const			{ return _outVolume; }
	float				getInLPF() const				{ return _inLPF; }
	float				getOutLPF() const				{ return _outLPF; }
	float				getInReverb() const				{ return _inReverb; }
	float				getOutReverb() const			{ return _outReverb; }

	void				setInAngle(float a)				{ _inAngle = a; }
	void				setOutAngle(float a)			{ _outAngle = a; }
	void				setInVolume(float v)			{ _inVolume = v; }
	void				setOutVolume(float v)			{ _outVolume = v; }
	void				setInLPF(float l)				{ _inLPF = l; }
	void				setOutLPF(float l)				{ _outLPF = l; }
	void				setInReverb(float r)			{ _inReverb = r; }
	void				setOutReverb(float r)			{ _outReverb = r; }
};

#endif