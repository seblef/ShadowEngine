
#ifndef _TANIMKEY_H_
#define _TANIMKEY_H_

namespace Core
{

template<typename T>
class TAnimKey
{
protected:

	float				_time;
	T					_val;

public:

	TAnimKey()			{}
	TAnimKey(float time, const T& val) : _time(time), _val(val)			{}
	TAnimKey(const TAnimKey<T>& k) : _time(k._time), _val(k._val)		{}

	TAnimKey<T>&		operator=(const TAnimKey& k)
	{
		_time=k._time;
		_val=k._val;
		return *this;
	}

	void				setTime(float time)					{ _time=time; }
	void				setValue(const T& val)				{ _val=val; }

	float				getTime() const						{ return _time; }
	const T&			getValue() const					{ return _val; }
};

};

#endif