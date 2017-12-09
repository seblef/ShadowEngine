
#ifndef _SOUND_H_
#define _SOUND_H_

#include <MediaCommon.h>
#include <XAudio2.h>

class Sound : public ISound
{
protected:

public:

	Sound(const string& soundName) : ISound(soundName)			{}
	virtual ~Sound()											{}

	virtual void				submitToVoice(IXAudio2SourceVoice* voice, bool loop) = 0;
};

#endif