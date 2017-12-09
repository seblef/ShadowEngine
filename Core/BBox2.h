
#ifndef _BBOX2_H_
#define _BBOX2_H_

#include "Vector2.h"
#include "CoreCommon.h"

namespace Core
{

class BBox2
{
protected:

	Vector2			_min, _max;

public:

	static BBox2	BoundBox;
	static BBox2	NullBox;

	BBox2() : _min(BoundBox._min), _max(BoundBox._max)			{}
	BBox2(const Vector2& min, const Vector2& max) : _min(min), _max(max)	{}
	BBox2(const BBox2& b) : _min(b._min), _max(b._max)			{}
	BBox2(float xmin, float ymin, float xmax, float ymax) : _min(xmin,ymin), _max(xmax,ymax) {}
	~BBox2()													{}

	const Vector2&	getMin() const								{ return _min; }
	const Vector2&	getMax() const								{ return _max; }
	void			getMinMax(Vector2& min, Vector2& max)		{ min=_min; max=_max; }

	void			setMin(const Vector2& min)					{ _min=min; }
	void			setMax(const Vector2& max)					{ _max=max; }
	void			setMinMax(const Vector2& min, const Vector2& max)	{ _min=min; _max=max; }

	BBox2&			operator=(const BBox2& b)					{ _min=b._min; _max=b._max; return *this; }

	BBox2			operator+(const Vector2& v)	const			{ return BBox2(_min+v, _max+v); }
	BBox2			operator-(const Vector2& v)	const			{ return BBox2(_min-v, _max-v); }

	BBox2&			operator+=(const Vector2& v)				{ _min+=v; _max+=v; return *this; }
	BBox2&			operator-=(const Vector2& v)				{ _min-=v; _max-=v; return *this; }

	BBox2&			operator << (const Vector2& v)
	{
		_min.x=smin(_min.x,v.x); _min.y=smin(_min.y,v.y);
		_max.x=smax(_max.x,v.x); _max.y=smax(_max.y,v.y);
		return *this;
	}
	BBox2&			operator << (const BBox2& b)
	{
		return (*this) << b._min << b._max;
	}

	bool			isIn(const Vector2& v) const				{ return (v.x >= _min.x && v.x <= _max.x) && (v.y >= _min.y && v.y <= _max.y); }
	bool			isIn(const BBox2& b) const					{ return !(b._max.x < _min.x || b._min.x > _max.x || b._max.y < _min.y || b._min.y > _max.y); }
	bool			isCompletlyIn(const BBox2& b) const			{ return b._min.x >= _min.x && b._max.x <= _max.x && b._min.y >= _min.y && b._max.y <= _max.y; }

	void			repair()
	{
		if(_min.x > _max.x)			sswap(_min.x,_max.x);
		if(_min.y > _max.y)			sswap(_min.y,_max.y);
	}

	Vector2			getSize() const								{ return _max - _min; }
	void			getSize(Vector2& size) const				{ size=_max-_min; }

	Vector2			getCenter() const							{ return (_max + _min) * 0.5f; }
	void			getCenter(Vector2& c) const					{ c=(_max + _min) * 0.5f; }
};

inline ostream& operator << (ostream& os, const BBox2& b)
{
	os << b.getMin() << " <--> " << b.getMax();
}

};

#endif