
#ifndef _TANIMTRACK_H_
#define _TANIMTRACK_H_

#include "TAnimKey.h"
#include "TTab.h"

namespace Core
{

template<typename T, typename S>
class TAnimTrack
{
protected:

	TTab<TAnimKey<T>>				_keys;

public:

	TAnimTrack(int keyCount) : _keys(keyCount)				{}
	TAnimTrack(int keyCount, const float *timeTab, const T* valTab) : _keys(keyCount)
	{
		for(int i=0;i<keyCount;++i)
		{
			_keys[i].setTime(timeTab[i]);
			_keys[i].setValue(valTab[i]);
		}
	}
	TAnimTrack(int keyCount, const TAnimKey<T>* keys) : _keys(keyCount,keys)			{}
	TAnimTrack(const TTab<TAnimKey<T>>& t) : _keys(t)									{}
	TAnimTrack(const TAnimTrack<T,S>& t) : _keys(t._keys)													{}	

	TTab<TAnimKey<T>>&				getKeys()						{ return _keys; }
	const TTab<TAnimKey<T>>&		getKeys() const					{ return _keys; }

	TAnimKey<T>&					getKey(int n)					{ return _keys[n]; }
	const TAnimKey<T>&				getKey(int n) const				{ return _keys[n]; }

	float							getTrackTime() const			{ return _keys[_keys.getCount()-1].getTime(); }
	int								getKeyCount() const				{ return _keys.getCount(); }

	void							evaluate(float time, T& val, bool loop, bool reverse) const
	{
		float animTime=getTrackTime();

		if(time <= 0.0f)
			val=_keys[0].getValue();
		else if(time >= animTime && !loop)
			val=_keys[_keys.getCount()-1].getValue();
		else
		{
			while(time > animTime)
				time-=animTime;

			if(reverse)			time=animTime-time;

			AnimKey<T> *prev,*next;
			int k=1;

			while(time > _keys[k-1] && k < _keys.getCount())
				++k;

			next=&_keys[k];
			prev=&_keys[k-1];

			float timeFact=(time - prev->getTime()) / (next->getTime() - prev->getTime());
			S::evaluate(prev->getValue(),next->getValue(),timeFact,val);
		}
	}
};

};

#endif