
#include "stdafx.h"
#include "SoundWAV.h"

SoundWAV::SoundWAV(const string& soundName, WAVEFORMATEX *checkFormat) : Sound(soundName), _format(0), _size(0), _data(0)
{
	HMMIO						hmmio;
	MMCKINFO					ck;
	MMCKINFO					ckRiff;

	//			Open
	hmmio = mmioOpen((LPSTR)_soundName.c_str(), 0, MMIO_ALLOCBUF | MMIO_READ);
	assert(hmmio != 0);

	readMMIO(hmmio,ckRiff);

	if (checkFormat)
	{
		assert(_format->nChannels == checkFormat->nChannels && _format->nSamplesPerSec == checkFormat->nSamplesPerSec &&
			_format->wBitsPerSample == checkFormat->wBitsPerSample);
	}

	resetFile(hmmio,ck,ckRiff);
	_size = ck.cksize;

	_data = new unsigned char[_size];
	read(hmmio, ck, ckRiff);

	//			Close
	mmioClose(hmmio,0);
}

SoundWAV::~SoundWAV()
{
	if (_format)				delete[] _format;
	if (_data)					delete[] _data;
}

void SoundWAV::readMMIO(HMMIO hmmio, MMCKINFO& ckRiff)
{
	MMCKINFO ckIn;					// chunk info. for general use.
	PCMWAVEFORMAT pcmWaveFormat;	// Temp PCM structure to load in.

	memset(&ckIn, 0, sizeof(ckIn));

	mmioDescend(hmmio, &ckRiff, 0, 0);

	assert((ckRiff.ckid == FOURCC_RIFF) && (ckRiff.fccType == mmioFOURCC('W', 'A', 'V', 'E')));

	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hmmio, &ckIn, &ckRiff, MMIO_FINDCHUNK);

	assert(ckIn.cksize >= (LONG)sizeof(PCMWAVEFORMAT));

	mmioRead(hmmio, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat));

	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
	{
		_format = (WAVEFORMATEX*)new char[sizeof(WAVEFORMATEX)];
		memcpy(_format, &pcmWaveFormat, sizeof(pcmWaveFormat));
		_format->cbSize = 0;
	}
	else
	{
		WORD cbExtraBytes = 0;
		mmioRead(hmmio, (char*)&cbExtraBytes, sizeof(WORD));

		_format = (WAVEFORMATEX*)new char[sizeof(WAVEFORMATEX)+cbExtraBytes];
		memcpy(_format, &pcmWaveFormat, sizeof(pcmWaveFormat));
		_format->cbSize = cbExtraBytes;

		mmioRead(hmmio, (char*)(((unsigned char*)&(_format->cbSize)) + sizeof(WORD)), cbExtraBytes);
	}

	mmioAscend(hmmio, &ckIn, 0);
}

void SoundWAV::resetFile(HMMIO hmmio, MMCKINFO& ck, MMCKINFO& ckRiff)
{
	mmioSeek(hmmio, ckRiff.dwDataOffset + sizeof(FOURCC), SEEK_SET);
	ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hmmio, &ck, &ckRiff, MMIO_FINDCHUNK);
}

void SoundWAV::read(HMMIO hmmio, MMCKINFO& ck, MMCKINFO& ckRiff)
{
	MMIOINFO mmioinfoIn; // current status of m_hmmio

	mmioGetInfo(hmmio, &mmioinfoIn, 0);

	for (unsigned long c = 0; c < _size; ++c)
	{
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
		{
			mmioAdvance(hmmio, &mmioinfoIn, MMIO_READ);
//			assert(mmioinfoIn.pchNext == mmioinfoIn.pchEndRead);
		}

		*((unsigned char*)_data + c) = *((unsigned char*)mmioinfoIn.pchNext);
		++mmioinfoIn.pchNext;
	}

	mmioSetInfo(hmmio, &mmioinfoIn, 0);
}

void SoundWAV::submitToVoice(IXAudio2SourceVoice* voice, bool loop)
{
	XAUDIO2_BUFFER buffer = { 0 };

	buffer.pAudioData =_data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = _size;
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	voice->SubmitSourceBuffer(&buffer);
}