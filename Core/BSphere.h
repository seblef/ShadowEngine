
#ifndef _BSPHERE_H_
#define _BSPHERE_H_

#include "Vector3.h"

namespace Core
{

class BSphere
{
protected:

	Vector3				_center;
	float				_radius;

public:

	BSphere() : _center(Vector3::NullVector), _radius(0)		{}
	BSphere(const Vector3& center, float radius) : _center(center), _radius(radius)	{}
	BSphere(const BSphere& b) : _center(b._center), _radius(b._radius)	{}
	BSphere(const Vector3& min, const Vector3& max) : _center((min+max)*0.5f), _radius(((max-min) * 0.5f).getLength())	{}
	~BSphere()		{}

	BSphere&			operator=(const BSphere& b)				{ _center=b._center; _radius=b._radius; return *this; }

	const Vector3&		getCenter() const						{ return _center; }
	float				getRadius() const						{ return _radius; }

	void				setCenter(const Vector3& c)				{ _center=c; }
	void				setRadius(float r)						{ _radius=r; }

	float				getDistanceFrom(const Vector3& v) const	{ return _center.getDistanceFrom(v) - _radius; }
	float				getDistanceFrom(const BSphere& b) const	{ return getDistanceFrom(b._center) - b._radius; }

	BSphere&			operator << (const Vector3& v)
	{
		float sq_len=(v - _center).getSquareLength();
		if(sq_len > _radius * _radius)
			_radius=sqrtf(sq_len);

		return *this;
	}
	BSphere&			operator << (const BSphere& b)
	{
		float sq_len=(b._center - _center).getSquareLength() + b._radius * b._radius;
		if(sq_len > _radius * _radius)
			_radius=sqrtf(sq_len);

		return *this;
	}

	bool				isIn(const Vector3& p) const			{ return getDistanceFrom(p) <= 0; }
	bool				isIn(const BSphere& b) const			{ return getDistanceFrom(b) <= 0; }
	bool				isIn(const Vector3& min, const Vector3& max) const
	{
		return _center.x + _radius >= min.x && _center.x - _radius <= max.x &&
			   _center.y + _radius >= min.y && _center.y - _radius <= max.y &&
			   _center.z + _radius >= min.z && _center.z - _radius <= max.z;
	}

	bool				isCompletlyIn(const BSphere& b) const	{ return getDistanceFrom(b) <= -b._radius; }
	bool				isCompletlyIn(const Vector3& min, const Vector3& max) const
	{
		return _center.x - _radius >= min.x && _center.x + _radius <= max.x &&
			   _center.y - _radius >= min.y && _center.y + _radius <= max.y &&
			   _center.z - _radius >= min.z && _center.z + _radius <= max.z;
	}

};

}
#endif