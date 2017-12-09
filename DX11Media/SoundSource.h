
#ifndef _SOUNDSOURCE_H_
#define _SOUNDSOURCE_H_

#include <X3DAudio.h>
#include <XAudio2.h>
#include "Sound.h"
#include "SoundVirtualSource.h"

class SoundSource : public ISoundSource, public IXAudio2VoiceCallback
{
protected:

	IXAudio2SourceVoice*			_voice;
	IXAudio2SubmixVoice*			_effect;
	IXAudio2MasteringVoice*			_master;
	X3DAUDIO_HANDLE*				_h3d;
	DXSoundVirtualSource*			_source;
	X3DAUDIO_DSP_SETTINGS			_dsp;
	float							_matrix[2];

	bool							_playing;
	bool							_loop;

public:

	SoundSource(IXAudio2* audio, IXAudio2SubmixVoice* effect,
		IXAudio2MasteringVoice* master,X3DAUDIO_HANDLE *h3d, WAVEFORMATEX* format);
	~SoundSource();

	void					play(ISoundVirtualSource* virtSrc, ISound* buffer, bool loop=false);
	void					stop();

#ifdef _DEBUG_SOUND
	void					onStop();
	void					onStart();
#endif
	void					update(const X3DAUDIO_LISTENER& listener);

	bool					isPlaying()					{ return _playing; }

	//				Callback

	STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32)
	{
	}
	STDMETHOD_(void, OnVoiceProcessingPassEnd)()
	{
	}
	STDMETHOD_(void, OnStreamEnd)()
	{
	}
	STDMETHOD_(void, OnBufferStart)(void*)
	{
		_playing = true;
#ifdef _DEBUG_SOUND
		onStart();
#endif
	}
	STDMETHOD_(void, OnBufferEnd)(void*)
	{
		if (!_loop)
		{
			_playing = false;
#ifdef _DEBUG_SOUND
			onStop();
#endif
		}
	}
	STDMETHOD_(void, OnLoopEnd)(void*)
	{
	}
	STDMETHOD_(void, OnVoiceError)(void*, HRESULT)
	{
	}

};

#endif