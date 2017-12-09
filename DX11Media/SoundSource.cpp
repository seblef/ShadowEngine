
#include "stdafx.h"
#include "SoundSource.h"

extern ofstream g_MediaOut;

SoundSource::SoundSource(IXAudio2* audio, IXAudio2SubmixVoice* effect,
	IXAudio2MasteringVoice* master, X3DAUDIO_HANDLE *h3d, WAVEFORMATEX* format) : _source(0), _effect(effect), _master(master), _h3d(h3d), _loop(false)
{
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[0].pOutputVoice = master;
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
	sendDescriptors[1].pOutputVoice = effect;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	// create the source voice
	audio->CreateSourceVoice(&_voice, format, 0, 2.0f, this, &sendList);

	memset(&_dsp, 0, sizeof(_dsp));
	_dsp.SrcChannelCount = 1;
	_dsp.DstChannelCount = 2;
	_dsp.pMatrixCoefficients = _matrix;
}

SoundSource::~SoundSource()
{
	_voice->DestroyVoice();
}

void SoundSource::play(ISoundVirtualSource* virtSrc, ISound *snd, bool loop)
{
#ifdef _DEBUG_SOUND
	g_MediaOut << "Playing sound " << snd->getSoundName() << " on source " << this << " (virtual: " << virtSrc << ")" << endl;
#endif
	if (isPlaying())
	{
#ifdef _DEBUG_SOUND
		g_MediaOut << " - Source already playing! Stopping..." << endl;
#endif
		stop();
	}

	_loop = loop;
	_source = (DXSoundVirtualSource*)virtSrc;
	((Sound*)snd)->submitToVoice(_voice, loop);

	_voice->SetVolume(_source->getVolume());
	_voice->Start();

	_playing = true;
}

void SoundSource::stop()
{
	_voice->Stop();
	_voice->FlushSourceBuffers();
	_source = 0;
	_loop = false;
}

void SoundSource::update(const X3DAUDIO_LISTENER& listener)
{
	if (_source && _source->isRelative())
	{
		DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
			| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
			| X3DAUDIO_CALCULATE_REVERB;

		X3DAudioCalculate(*_h3d, &listener, &_source->getEmitter(), dwCalcFlags,
			&_dsp);

		_voice->SetFrequencyRatio(_dsp.DopplerFactor);
		_voice->SetOutputMatrix(_master, 1, 2, _matrix);
		_voice->SetOutputMatrix(_effect, 1, 1, &_dsp.ReverbLevel);
		XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * _dsp.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
		_voice->SetOutputFilterParameters(_master, &FilterParametersDirect);
		XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * _dsp.LPFReverbCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
		_voice->SetOutputFilterParameters(_effect, &FilterParametersReverb);

	}
}

#ifdef _DEBUG_SOUND
void SoundSource::onStop()
{
	g_MediaOut << "Source " << this << " has finished." << endl;
}

void SoundSource::onStart()
{
	g_MediaOut << "Source " << this << " is starting." << endl;
}
#endif