
#ifndef _SOUNDVIRTUALSOURCE_H_
#define _SOUNDVIRTUALSOURCE_H_

#include "SoundCone.h"
#include "SoundUtil.h"

class DXSoundVirtualSource : public ISoundVirtualSource
{
protected:

	X3DAUDIO_EMITTER			_emitter;
	SoundCone*					_cone;
	bool						_relative;
	float						_volume;

public:

	DXSoundVirtualSource() : _cone(0), _relative(true), _volume(1)
	{
		_emitter.ChannelCount = 1;
		_emitter.pChannelAzimuths = 0;
		_emitter.pCone = 0;
		_emitter.pVolumeCurve = 0;
		_emitter.pLFECurve = 0;
		_emitter.pLPFDirectCurve = 0;
		_emitter.pLPFReverbCurve = 0;
		_emitter.pReverbCurve = 0;
		_emitter.DopplerScaler = 1.0f;
		_emitter.CurveDistanceScaler = 1.0;
	}
	DXSoundVirtualSource(const Vector3& pos, const Vector3& vel,
		float inRadius, float inRadiusAngle, float volume=1.0f,
		SoundCone *cone=0, const Vector3& frontAxis = Vector3::ZAxisVector,
		const Vector3& topAxis = Vector3::YAxisVector,
		float dopplerScale = 1.0f) : _relative(true), _volume(volume)
	{
		_emitter.ChannelCount = 1;
		_emitter.pChannelAzimuths = 0;
		_emitter.pCone = 0;
		_emitter.pVolumeCurve = 0;
		_emitter.pLFECurve = 0;
		_emitter.pLPFDirectCurve = 0;
		_emitter.pLPFReverbCurve = 0;
		_emitter.pReverbCurve = 0;
		_emitter.CurveDistanceScaler = 1.0;

		soundCopyVec(pos, _emitter.Position);
		soundCopyVec(vel, _emitter.Velocity);
		_emitter.InnerRadius = inRadius;
		_emitter.InnerRadiusAngle = inRadiusAngle;
		soundCopyVec(frontAxis, _emitter.OrientFront);
		soundCopyVec(topAxis, _emitter.OrientTop);
		_emitter.DopplerScaler = dopplerScale;

		if (cone)
		{
			_emitter.pCone = ((SoundCone*)cone)->getCone();
			_cone = (SoundCone*)cone;
		}
	}

	void						setPosition(const Vector3& pos)			{ soundCopyVec(pos, _emitter.Position); }
	void						setVelocity(const Vector3& vel)			{ soundCopyVec(vel, _emitter.Velocity); }
	void						setInRadius(float r)					{ _emitter.InnerRadius = r; }
	void						setInRadiusAngle(float a)				{ _emitter.InnerRadiusAngle = a; }
	void						setCone(ISoundCone* c);
	void						setFrontAxis(const Vector3& f)			{ soundCopyVec(f, _emitter.OrientFront); }
	void						setTopAxis(const Vector3& t)			{ soundCopyVec(t, _emitter.OrientTop); }
	void						setDopplerScale(float d)				{ _emitter.DopplerScaler = d; }
	void						setRelative(bool relative)				{ _relative = relative; }
	void						setVolume(float v)						{ _volume = v; }

	const Vector3&				getPosition() const						{ return SOUND_RETURN_VEC_CONST(_emitter.Position); }
	const Vector3&				getVelocity() const						{ return SOUND_RETURN_VEC_CONST(_emitter.Velocity); }
	float						getInRadius() const						{ return _emitter.InnerRadius; }
	float						getInRadiusAngle() const				{ return _emitter.InnerRadiusAngle; }
	ISoundCone*					getCone() const							{ return _cone; }
	const Vector3&				getFrontAxis() const					{ return SOUND_RETURN_VEC_CONST(_emitter.OrientFront); }
	const Vector3&				getTopAxis() const						{ return SOUND_RETURN_VEC_CONST(_emitter.OrientTop); }
	float						getDopplerScale() const					{ return _emitter.DopplerScaler; }
	bool						isRelative() const						{ return _relative; }
	float						getVolume() const						{ return _volume; }

	const X3DAUDIO_EMITTER&		getEmitter() const						{ return _emitter; }

#ifdef _DEBUG
	void						outputSource(ofstream& ofs) const;
#endif
};

#endif