
#ifndef _SOUNDVIRTUALSOURCE_H_
#define _SOUNDVIRTUALSOURCE_H_

#include <MediaCommon.h>


class SoundPlayingSource;

class SoundVirtualSource
{
	protected:

		int						_priority;
		ISoundSource*			_playingSource;
		ISoundVirtualSource*	_source;
		ISound*					_sound;

		float					_distance;
		float					_volume;
		bool					_loop;
		bool					_stopped;
		bool					_managed;

	public:

		SoundVirtualSource(ISoundDevice *d, int priority, float volume, bool loop, bool managed=false);
		~SoundVirtualSource()
		{
			delete _source;
		}

		bool					isPlaying() const				{ return _source != 0; }
		bool					isManaged() const				{ return _managed; }
		bool					isLooping() const				{ return _loop; }
		bool					isStopped() const				{ return _stopped; }
		int						getPriority() const				{ return _priority; }
		ISoundSource*			getPlayingSource()				{ return _playingSource; }
		float					getDistance() const				{ return _distance; }
		ISound*					getSound() const				{ return _sound; }
		ISoundVirtualSource*	getSource() const				{ return _source; }

		void					setStopped()									{ _stopped = true; }
		void					setPlayingSource(ISoundSource *source)			{ _playingSource=source; }
		void					setSound(ISound* snd)							{ _sound = snd;  }
		void					updateDistance(const Vector3& listener)
		{
			_distance = (_source->getPosition() - listener).getSquareLength();
		}
};

#endif
