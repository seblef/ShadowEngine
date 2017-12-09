
#ifndef _SOUNDPLAYINGSOURCE_H_
#define _SOUNDPLAYINGSOURCE_H_

#include <MediaCommon.h>

class SoundVirtualSource;

class SoundPlayingSource
{
protected:

	SoundVirtualSource*		_virtualSource;
	ISoundSource*			_actualSource;
	bool					_stopped;

public:

	SoundPlayingSource(SoundVirtualSource *virt) : _virtualSource(virt), _actualSource(0), _stopped(false) {}
	~SoundPlayingSource() {}

	void					setVirtualSource(SoundVirtualSource* src)			{ _virtualSource = src; }
	void					setActualSource(ISoundSource* src)					{ _actualSource = src; }
	void					setStopped(bool stopped)							{ _stopped = stopped; }

	SoundVirtualSource*		getVirtualSource() const							{ return _virtualSource; }
	ISoundSource*			getActualSource() const								{ return _actualSource; }
	bool					isStopped() const									{ return _stopped; }
};	

#endif