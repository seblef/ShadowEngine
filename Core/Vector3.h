
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "Matrix4.h"

namespace Core
{

class Vector3
{
public:

	float			x,y,z;

	static Vector3	NullVector;
	static Vector3	XAxisVector;
	static Vector3	YAxisVector;
	static Vector3	ZAxisVector;
	static Vector3	UnitVector;

	Vector3()		{}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(const Vector3& v) : x(v.x),y(v.y),z(v.z) {}
	Vector3(const float *pv) : x(*pv), y(*(pv+1)), z(*(pv+2)) {}
	Vector3(const Matrix4& m) : x(m(3,0)), y(m(3,1)), z(m(3,2))	{}

	~Vector3() {}

	Vector3&		operator=(const Vector3& v)				{x=v.x; y=v.y; z=v.z; return *this; }
	Vector3&		operator=(const Matrix4& m)				{ x=m(3,0); y=m(3,1); z=m(3,2); return *this; }

	bool			operator==(const Vector3& v) const		{ return ((v.x==x) && (v.y==y) && (v.z==z)); }
	bool			operator!=(const Vector3& v) const		{ return ((v.x!=x) || (v.y!=y) || (v.z!=z)); }

	float			operator[](int i) const					{ return ((float*)&x)[i]; }
	float&			operator[](int i)						{ return ((float*)&x)[i]; }

	Vector3			operator-() const						{ return Vector3(-x,-y,-z); }

	Vector3			operator+(const Vector3& v) const		{ return Vector3(x+v.x, y+v.y, z+v.z); }
	Vector3			operator+(float f) const				{ return Vector3(x+f,y+f,z+f); }
	Vector3&		operator+=(const Vector3& v)			{ x+=v.x; y+=v.y; z+=v.z; return *this; }
	Vector3&		operator+=(float f)						{ x+=f; y+=f; z+=f; return *this; }

	Vector3			operator-(const Vector3& v) const		{ return Vector3(x-v.x, y-v.y, z-v.z); }
	Vector3			operator-(float f) const				{ return Vector3(x-f,y-f,z-f); }
	Vector3&		operator-=(const Vector3& v)			{ x-=v.x; y-=v.y; z-=v.z; return *this; }
	Vector3&		operator-=(float f)						{ x-=f; y-=f; z-=f; return *this; }

	float			operator*(const Vector3& v) const		{ return x*v.x + y*v.y + z*v.z; }

	Vector3			operator*(float f) const				{ return Vector3(x*f, y*f, z*f); }
	Vector3&		operator*=(const float f)				{ x*=f; y*=f; z*=f; return *this; }
	Vector3&		operator*=(const Vector3& v)			{ x*=v.x; y*=v.y; z*=v.z; return *this; }
	Vector3			operator*(const Matrix4& m) const
	{
		return Vector3(x*m(0,0) + y*m(1,0) + z*m(2,0) + m(3,0),
					   x*m(0,1) + y*m(1,1) + z*m(2,1) + m(3,1),
					   x*m(0,2) + y*m(1,2) + z*m(2,2) + m(3,2));
	}
	Vector3&		operator*=(const Matrix4& m)
	{
		float tx=x;
		float ty=y;
		float tz=z;

		x=tx*m(0,0) + ty*m(1,0) + tz*m(2,0) + m(3,0);
		y=tx*m(0,1) + ty*m(1,1) + tz*m(2,1) + m(3,1);
		z=tx*m(0,2) + ty*m(1,2) + tz*m(2,2) + m(3,2);

		return *this;
	}

	Vector3			operator/(float f) const				{ return Vector3(x/f, y/f, z/f); }
	Vector3&		operator/=(float f)						{ x/=f; y/=f; z/=f; return *this; }

	Vector3			operator^(const Vector3& v) const		{ return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }

	float			getSquareLength() const					{ return x*x + y*y + z*z; }
	float			getLength() const						{ return sqrtf(x*x + y*y + z*z); }

	void			normalize()
	{
		const float fNorm=x*x+y*y+z*z;
		if((FloatMakePositive(fNorm-1.0f) > SQREPSILON) && (fNorm > SQREPSILON))
		{
			const float fInvNorm=FastInvSqrt(fNorm);
			x*=fInvNorm; y*=fInvNorm; z*=fInvNorm;
		}
	}

	void			negativeThis() { x=-x; y=-y; z=-z; }

	float			getDistanceFromSqr(const Vector3& v) const
	{
		const float dx=x-v.x;
		const float dy=y-v.y;
		const float dz=z-v.z;
		return dx*dx+dy*dy+dz*dz;
	}
	float			getDistanceFrom(const Vector3& v) const
	{
		return sqrtf(getDistanceFromSqr(v));
	}

	bool			isBetween(const Vector3& v1, const Vector3& v2) const
	{
		float f=(v2-v1).getSquareLength();
		return (getDistanceFromSqr(v1) < f && getDistanceFromSqr(v2) < f);
	}

	void			transformNoTranslation(const Matrix4& m)
	{
		float tx=x;
		float ty=y;
		float tz=z;

		x=tx*m(0,0) + ty*m(1,0) + tz*m(2,0);
		y=tx*m(0,1) + ty*m(1,1) + tz*m(2,1);
		z=tx*m(0,2) + ty*m(1,2) + tz*m(2,2);
	}

	void			scaleFrom(const Vector3& v, const Vector3& vScale)			{ x=v.x*vScale.x; y=v.y*vScale.y; z=v.z*vScale.z; }
	void			invScale(const Vector3& vScale)							{ x/=vScale.x; y/=vScale.y; z/=vScale.z; }
	void			invScaleFrom(const Vector3& v, const Vector3& vScale)		{ x=v.x/vScale.x; y=v.y/vScale.y; z=v.z/vScale.z; }
	void			inverse()													{ x=1.0f / x; y=1.0f / y; z=1.0f / z; }
	void			inverseFrom(const Vector3& v)								{ x=1.0f / v.x;	y=1.0f / v.y; z=1.0f / v.z; }

	static void		slerp(const Vector3& v1, const Vector3& v2, float t, Vector3& ret)
	{
		ret=v2;
		ret-=v1;
		ret*=t;
		ret+=v1;
	}
};

inline Vector3 operator*(float f, const Vector3& v)
{
	return v*f;
}

inline ostream& operator << (ostream& os, const Vector3& v)
{
	os << "< " << v.x << " , " << v.y << " , " << v.z << " >";
	return os;
}

}

#endif