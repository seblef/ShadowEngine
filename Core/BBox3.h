
#ifndef _BBOX3_H_
#define _BBOX3_H_

#include "BSphere.h"
#include "CoreCommon.h"

namespace Core
{

class BBox3
{
protected:

	Vector3			_min,_max;

public:

	static BBox3	NullBox;
	static BBox3	BoundBox;
	static BBox3	InitBox;

	BBox3() : _min(BoundBox._min), _max(BoundBox._max)		{}
	BBox3(const Vector3& min, const Vector3& max) : _min(min), _max(max)	{}
	BBox3(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax) : _min(xmin,ymin,zmin), _max(xmax,ymax,zmax)		{}
	BBox3(const BBox3& b) : _min(b._min), _max(b._max)						{}
	BBox3(const BSphere& b) : _min(b.getCenter()-b.getRadius()), _max(b.getCenter()+b.getRadius())	{}
	~BBox3()												{}

	void			createCenterSize(const Vector3& c, const Vector3& s)
	{
		_min=c-(s*0.5f);
		_max=c+(s*0.5f);
	}

	const Vector3&	getMin() const								{ return _min; }
	const Vector3&	getMax() const								{ return _max; }
	void			getMinMax(Vector3& min, Vector3& max) const	{ min=_min; max=_max; }

	void			setMin(const Vector3& min)					{ _min=min; }
	void			setMax(const Vector3& max)					{ _max=max; }
	void			setMinMax(const Vector3& min, const Vector3& max)	{ _min=min; _max=max; }

	Vector3			getCenter() const							{ return (_min + _max) * 0.5f; }
	void			getCenter(Vector3& c) const					{ c=(_min + _max) * 0.5f; }

	Vector3			getSize() const								{ return (_max-_min); }
	void			getSize(Vector3& s) const					{ s=(_max-_min); }

	BBox3&			operator=(const BBox3& b)					{ _min=b._min; _max=b._max; return *this; }
	BBox3&			operator=(const BSphere& b)					{ _min=b.getCenter()-b.getRadius(); _max=b.getCenter()+b.getRadius(); return *this; }

	BBox3			operator+(const Vector3& v) const			{ return BBox3(_min+v,_max+v); }
	BBox3			operator-(const Vector3& v)	const			{ return BBox3(_min-v,_max-v); }

	BBox3&			operator+=(const Vector3& v)				{ _min+=v; _max+=v; return *this; }
	BBox3&			operator-=(const Vector3& v)				{ _min-=v; _max-=v; return *this; }

	BBox3&			operator << (const Vector3& v)
	{
		_min.x=smin(_min.x,v.x); _min.y=smin(_min.y,v.y); _min.z=smin(_min.z,v.z);
		_max.x=smax(_max.x,v.x); _max.y=smax(_max.y,v.y); _max.z=smax(_max.z,v.z);
		return *this;
	}

	BBox3&			operator << (const BBox3& b)
	{
		return (*this) << b._min << b._max;
	}

	BBox3&			operator << (const BSphere& b)				{ return (*this) << BBox3(b); }

	BBox3			operator+(const BBox3& b) const
	{
		return BBox3(smin(_min.x,b._min.x),smin(_min.y,b._min.y),smin(_min.z,b._min.z),
			smax(_max.x,b._max.x),smax(_max.y,b._max.y),smax(_max.z,b._max.z));
	}
	BBox3&			operator+=(const BBox3& b)					{ return (*this) << b; }

	BBox3			operator*(const Matrix4& m) const
	{
		Vector3 center(getCenter() * m);
		Vector3 size(getSize() * 0.5f);
		size.transformNoTranslation(m);
		return BBox3(center - size, center + size);
	}
	BBox3&			operator*=(const Matrix4& m)
	{
		Vector3 center(getCenter() * m);
		Vector3 size(getSize() * 0.5f);
		size.transformNoTranslation(m);
		_min=center - size;
		_max=center + size;
		return *this;
	}

	void			transformFromBox(const BBox3& b, const Matrix4& m)
	{
		Vector3 center(b.getCenter() * m);
		Vector3 size(b.getSize() * 0.5f);
		size.transformNoTranslation(m);
		_min=center - size;
		_max=center + size;
		repair();
	}

	bool			isValid() const						{ return _min.x < _max.x && _min.y < _max.y && _min.z < _max.z; }
	void			repair()
	{
		if(_min.x > _max.x)			sswap(_min.x,_max.x);
		if(_min.y > _max.y)			sswap(_min.y,_max.y);
		if(_min.z > _max.z)			sswap(_min.z,_max.z);
	}

	void			getSphere(BSphere& b) const
	{
		b.setCenter(getCenter());
		b.setRadius(getSize().getLength() * 0.5f);
	}

	bool			isIn(const Vector3& v) const		{ return v.x >= _min.x && v.y >= _min.y && v.z >= _min.z && v.x <= _max.x && v.y <= _max.y && v.z <=_max.z; }
	bool			isIn(const BBox3& b) const
	{
		return (b._max.x >= _min.x && b._min.x <= _max.x) && (b._max.y >= _min.y && b._min.y <= _max.y) && (b._max.z >= _min.z && b._min.z <= _max.z);
	}
	bool			isIn(const Vector3& c, float r) const
	{
		return c.x+r >= _min.x && c.x-r <= _max.x &&
			   c.y+r >= _min.y && c.y-r <= _max.y &&
			   c.z+r >= _min.z && c.z-r <= _max.z;
	}

	bool			isCompletlyIn(const BBox3& b) const
	{
		return (b._min.x >= _min.x && b._min.y >= _min.y && b._min.z >= _min.z && b._max.x <= _max.x && b._max.y <= _max.y && b._max.z <= _max.z);
	}
	bool			isCompletlyIn(const Vector3& c, float r) const
	{
		return c.x-r >= _min.x && c.x+r <= _max.x &&
			   c.y-r >= _min.y && c.y+r <= _max.y &&
			   c.z-r >= _min.z && c.z+r <= _max.z;
	}

	bool			rayIntersect(const Vector3& o, const Vector3& d) const;

	float			getDistanceFromSq(const Vector3& p) const
	{
		Vector3 delta(Vector3::NullVector);

		if(p.x < _min.x) delta.x=_min.x - p.x;
		else if(p.x > _max.x) delta.x=p.x - _max.x;

		if(p.y < _min.y) delta.y=_min.y - p.y;
		else if(p.y > _max.y) delta.y=p.y - _max.y;

		if(p.z < _min.z) delta.z=_min.z - p.z;
		else if(p.z > _max.z) delta.z=p.z - _max.z;

		return delta.getSquareLength();
	}

	float			getDistanceFrom(const Vector3& p) const		{ return sqrtf(getDistanceFromSq(p)); }

	void			getPoints(Vector3 *pv) const
	{
		pv[0]=_min;
		pv[1].x=_min.x;			pv[1].y=_min.y;			pv[1].z=_max.z;
		pv[2].x=_min.x;			pv[2].y=_max.y;			pv[2].z=_min.z;
		pv[3].x=_min.x;			pv[3].y=_max.y;			pv[3].z=_max.z;
		pv[4].x=_max.x;			pv[4].y=_min.y;			pv[4].z=_min.z;
		pv[5].x=_max.x;			pv[5].y=_min.y;			pv[5].z=_max.z;
		pv[6].x=_max.x;			pv[6].y=_max.y;			pv[6].z=_min.z;
		pv[7]=_max;
	}
};

inline ostream& operator << (ostream& os, const BBox3& b)
{
	os << b.getMin() << " <--> " << b.getMax();
	return os;
}

};

#endif