
#ifndef _ISOUNDDEVICE_H_
#define _ISOUNDDEVICE_H_

#include "ISoundSource.h"
#include "ISoundVirtualSource.h"
#include "ISoundListener.h"

class ISoundDevice
{
public:

	ISoundDevice()				{}
	virtual ~ISoundDevice()		{}

	virtual ISound*						loadSound(const string& soundFile)=0;

	virtual void						update()=0;

	virtual ISoundListener*				getListener() = 0;

	virtual int							getSourceCount() const = 0;
	virtual ISoundSource*				getSource(int n) const = 0;

	virtual ISoundCone*					createCone() const = 0;
	virtual ISoundCone*					createCone(float iAngle, float oAngle, float iVolume, float oVolume,
		float iLPF, float oLPF, float iReverb, float oReverb) const = 0;

	virtual ISoundVirtualSource*		createVirtualSource() const = 0;
	virtual ISoundVirtualSource*		createVirtualSource(const Vector3& pos, const Vector3& vel,
		float inRadius, float inRadiusAngle, float volume=1.0f,
		ISoundCone *cone=0, const Vector3& frontAxis = Vector3::ZAxisVector,
		const Vector3& topAxis = Vector3::YAxisVector,
		float dopplerScale = 1.0f) const = 0;
};

#endif