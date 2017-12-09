
#include "stdafx.h"
#include "SoundDevice.h"
#include "SoundWAV.h"
#include <XAudio2fx.h>

#define SOUND_MAX_SOURCES					16

SoundDevice::SoundDevice() : _sourceCount(SOUND_MAX_SOURCES)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	XAudio2Create(&_audio, flags);
	_audio->CreateMasteringVoice(&_master);

	XAUDIO2_DEVICE_DETAILS details;
	_audio->GetDeviceDetails(0, &details);
	assert(details.OutputFormat.Format.nChannels == 2);

	//
	// Create reverb effect
	//
	flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2FX_DEBUG;
#endif

	XAudio2CreateReverb(&_reverbEffect, flags);

	// Performance tip: you need not run global FX with the sample number
	// of channels as the final mix.  For example, this sample runs
	// the reverb in mono mode, thus reducing CPU overhead.
	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { _reverbEffect, TRUE, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

	_audio->CreateSubmixVoice(&_effect, 1,
		details.OutputFormat.Format.nSamplesPerSec, 0, 0,
		NULL, &effectChain);

	// Set default FX params
	XAUDIO2FX_REVERB_PARAMETERS native;
	XAUDIO2FX_REVERB_I3DL2_PARAMETERS preset = XAUDIO2FX_I3DL2_PRESET_DEFAULT;
	ReverbConvertI3DL2ToNative(&preset, &native);
	_effect->SetEffectParameters(0, &native, sizeof(native));

	//
	// Initialize X3DAudio
	//  Speaker geometry configuration on the final mix, specifies assignment of channels
	//  to speaker positions, defined as per WAVEFORMATEXTENSIBLE.dwChannelMask
	//
	//  SpeedOfSound - speed of sound in user-defined world units/second, used
	//  only for doppler calculations, it must be >= FLT_MIN
	//
	const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

	X3DAudioInitialize(details.OutputFormat.dwChannelMask, SPEEDOFSOUND, _audio3d);

	SoundWAV check("Sounds/Format.wav", 0);
	memcpy(&_format, check.getFormat(), sizeof(WAVEFORMATEX));

	_sources = new SoundSource*[_sourceCount];
	for (int i = 0; i < _sourceCount; ++i)
		_sources[i] = new SoundSource(_audio, _effect, _master, &_audio3d, &_format);
}

SoundDevice::~SoundDevice()
{
	for (int i = 0; i < _sourceCount; ++i)
	{
		if (_sources[i]->isPlaying())
			_sources[i]->stop();

		delete _sources[i];
	}

	delete[] _sources;

	_effect->DestroyVoice();
	_master->DestroyVoice();

	_audio->StopEngine();
	_reverbEffect->Release();
	_audio->Release();

	CoUninitialize();
}

ISound* SoundDevice::loadSound(const string& soundFile)
{
	string ext = soundFile.substr(soundFile.length() - 3, 3);
	if (ext == "wav" || ext == "WAV")
		return new SoundWAV(soundFile, &_format);
	else
		return 0;
}

void SoundDevice::update()
{
	for (int i = 0; i < _sourceCount; ++i)
	{
		if (_sources[i]->isPlaying())
			_sources[i]->update(_listener.getListener());
	}
}