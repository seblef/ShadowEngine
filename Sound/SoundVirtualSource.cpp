
#include "stdafx.h"
#include "SoundVirtualSource.h"
#include "SoundPlayingSource.h"


SoundVirtualSource::SoundVirtualSource(ISoundDevice *d, int priority, float volume, bool loop, bool managed) :
	_playingSource(0), _priority(priority), _loop(loop), _managed(managed), _sound(0), _stopped(false),
	_volume(volume)
{
	_source = d->createVirtualSource();
	_source->setVolume(volume);
}

