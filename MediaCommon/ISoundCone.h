
#ifndef _ISOUNDCONE_H_
#define _ISOUNDCONE_H_

class ISoundCone
{
public:

	ISoundCone()				{}
	virtual ~ISoundCone()		{}

	virtual void			setInnerAngle(float a) = 0;
	virtual void			setOuterAngle(float a) = 0;
	virtual void			setInnerVolume(float v) = 0;
	virtual void			setOuterVolume(float v) = 0;
	virtual void			setInnerLPF(float lpf) = 0;
	virtual void			setOuterLPF(float lpf) = 0;
	virtual void			setInnerReverb(float r) = 0;
	virtual void			setOuterReverb(float r) = 0;

	virtual float			getInnerAngle() = 0;
	virtual float			getOuterAngle() = 0;
	virtual float			getInnerVolume() = 0;
	virtual float			getOuterVolume() = 0;
	virtual float			getInnerReverb() = 0;
	virtual float			getOuterReverb() = 0;
	virtual float			getInnerLPF() = 0;
	virtual float			getOuterLPF() = 0;
};

#endif