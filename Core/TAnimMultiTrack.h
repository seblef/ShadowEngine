
#ifndef _TANIMMULTITRACK_H_
#define _TANIMMULTITRACK_H_

#include <memory.h>

namespace Core
{

template<typename T,typename S>
class TAnimMultiTrack
{
protected:

	int						_keyCount;
	int						_trackCount;
	float					*_time;
	T						*_values;

public:

	TAnimMultiTrack(int keyCount, int trackCount) : _keyCount(keyCount), _trackCount(trackCount)
	{
		_time=new float[keyCount];
		_values=new T[keyCount*trackCount];
	}
	TAnimMultiTrack(int keyCount, int trackCount, float *timeTab, T* valTab) :
		_keyCount(keyCount), _trackCount(trackCount), _time(timeTab), _values(valTab)	{}
	TAnimMultiTrack(const TAnimMultiTrack<T,S>& t) : _keyCount(t._keyCount), _trackCount(t._trackCount)
	{
		_time=new float[_keyCount];
		_values=new T[_keyCount*_trackCount];
		memcpy(_time,t._time,sizeof(float)*_keyCount);
		memcpy(_values,t._values,sizeof(T)*_keyCount*_trackCount);
	}
	~TAnimMultiTrack()
	{
		delete[] _time;
		delete[] _values;
	}

	int						getKeyCount() const					{ return _keyCount; }
	int						getTrackCount() const				{ return _trackCount; }
	float					getTrackTime() const				{ return _time[_keyCount-1]; }

	float					getTime(int key) const				{ return _time[key]; }
	void					setTime(int key, float time)		{ _time[key]=time; }

	T&						getValue(int key, int track)		{ return _values[key*_trackCount+track]; }
	const T&				getValue(int key, int track) const	{ return _values[key*_trackCount+track]; }

	float*					getTimes() const					{ return _time; }
	T*						getValues() const					{ return _values; }

	void					evaluate(float time, T* val, bool loop, bool reverse) const
	{
		float animTime=getTrackTime();

		if(time <= 0.0f)
			memcpy(val,_values,sizeof(T)*_trackCount);
		else if(time >= animTime && !loop)
			memcpy(val,&_values[(_keyCount-1)*_trackCount],sizeof(T)*_trackCount);
		else
		{
			while(time > animTime)
				time-=animTime;

			if(reverse)			time=animTime-time;

			T *prev,*next;
			int k=1;

			while(time > _time[k-1] && k < _keyCount-1)
				++k;

			next=&_values[k*_trackCount];
			prev=&_values[(k-1)*_trackCount];

			float nextt=_time[k];
			float prevt=_time[k-1];

			float timeFact=(time - prevt) / (nextt - prevt);
			for(int i=0;i<_trackCount;++i,++next,++prev,++val)
				S::evaluate(*prev,*next,timeFact,*val);
		}
	}

};

};

#endif