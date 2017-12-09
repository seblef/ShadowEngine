
#ifndef _IMEDIA_H_
#define _IMEDIA_H_

#include "IVideoDevice.h"
#include "ISoundDevice.h"
#include "EventPump.h"

class IMedia
{
protected:

	IVideoDevice*			_video;
	ISoundDevice*			_audio;

public:

	IMedia() : _video(0), _audio(0)
	{
		new EventPump;
	}

	virtual ~IMedia()
	{
		EventPump::deleteSingleton();
		delete _video;
		delete _audio;
	}

	IVideoDevice*			getVideo() const				{ return _video; }
	ISoundDevice*			getAudio() const				{ return _audio; }

	virtual void			update()=0;
};

#endif