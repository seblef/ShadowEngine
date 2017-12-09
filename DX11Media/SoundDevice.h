
#ifndef _SOUNDDEVICE_H_
#define _SOUNDDEVICE_H_

#include "SoundSource.h"
#include "SoundListener.h"

class SoundDevice : public ISoundDevice
{
protected:

	IXAudio2*					_audio;
	X3DAUDIO_HANDLE				_audio3d;
	IXAudio2SubmixVoice*		_effect;
	IXAudio2MasteringVoice*		_master;
	IUnknown*					_reverbEffect;
	WAVEFORMATEX				_format;

	SoundListener				_listener;
	int							_sourceCount;
	SoundSource**				_sources;

public:

	SoundDevice();
	~SoundDevice();

	ISound*						loadSound(const string& soundFile);

	void						update();

	ISoundListener*				getListener()						{ return &_listener; }

	int							getSourceCount() const				{ return _sourceCount; }
	ISoundSource*				getSource(int n) const				{ return _sources[n]; }

	ISoundCone*					createCone() const					{ return new SoundCone(); }
	ISoundCone*					createCone(float iAngle, float oAngle, float iVolume, float oVolume,
		float iLPF, float oLPF, float iReverb, float oReverb) const
	{
		return new SoundCone(iAngle, oAngle, iVolume, oVolume, iLPF, oLPF, iReverb, oReverb);
	}

	ISoundVirtualSource*		createVirtualSource() const			{ return new DXSoundVirtualSource(); }
	ISoundVirtualSource*		createVirtualSource(const Vector3& pos, const Vector3& vel,
		float inRadius, float inRadiusAngle, float volume=1.0f,
		ISoundCone *cone=0, const Vector3& frontAxis = Vector3::ZAxisVector,
		const Vector3& topAxis = Vector3::YAxisVector,
		float dopplerScale = 1.0f) const
	{
		return new DXSoundVirtualSource(pos, vel, inRadius, inRadiusAngle, volume, (SoundCone*)cone, frontAxis, topAxis, dopplerScale);
	}
};

#endif