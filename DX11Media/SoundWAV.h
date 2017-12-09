
#ifndef _SOUNDWAV_H_
#define _SOUNDWAV_H_

#include "Sound.h"
#include <mmsystem.h>


class SoundWAV : public Sound
{
protected:

	WAVEFORMATEX*				_format;
	unsigned int				_size;
	unsigned char*				_data;

	void						readMMIO(HMMIO hmmio, MMCKINFO& ckRiff);
	void						read(HMMIO hmmio, MMCKINFO& ck, MMCKINFO& ckRiff);
	void						resetFile(HMMIO hmmio, MMCKINFO& ck, MMCKINFO& ckRiff);

public:

	SoundWAV(const string& wavFile, WAVEFORMATEX *checkFormat);
	~SoundWAV();

	unsigned long				getSize() const					{ return _size; }
	WAVEFORMATEX*				getFormat() const				{ return _format; }
	unsigned char*				getData() const					{ return _data;  }

	void						submitToVoice(IXAudio2SourceVoice* voice, bool loop);
};

#endif