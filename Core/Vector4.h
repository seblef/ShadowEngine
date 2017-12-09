
#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include "Vector3.h"

namespace Core
{

class Vector4
{
public:

	static Vector4		NullVector;
	static Vector4		XAxisVector;
	static Vector4		YAxisVector;
	static Vector4		ZAxisVector;
	static Vector4		WAxisVector;
	static Vector4		UnitVector;

	static Vector4		Black;
	static Vector4		White;
	static Vector4		Red;
	static Vector4		Green;
	static Vector4		Blue;

	union { float x;	float r; };
	union { float y;	float g; };
	union { float z;	float b; };
	union { float w;	float a; };

	Vector4()					{}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)	{}
	Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w)		{}
	Vector4(const float *f) : x(f[0]), y(f[1]), z(f[2]), w(f[3])	{}
	Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(0.0f)		{}
	~Vector4()														{}

	Vector4&			operator=(const Vector4& v)					{x=v.x; y=v.y; z=v.z; w=v.w; return *this; }
	Vector4&			operator=(const Vector3& v)					{x=v.x; y=v.y; z=v.z; w=0.0f; return *this; }
	Vector4&			operator=(const Matrix4& m)					{ x=m(3,0); y=m(3,1); z=m(3,2); w=m(3,3); return *this; }

	bool				operator==(const Vector4& v) const			{ return ((v.x==x) && (v.y==y) && (v.z==z) && (v.w==w)); }
	bool				operator!=(const Vector4& v) const			{ return ((v.x!=x) || (v.y!=y) || (v.z!=z) || (v.w!=w)); }

	float				operator[](int i) const						{ return ((float*)&x)[i]; }
	float&				operator[](int i)							{ return ((float*)&x)[i]; }

	Vector4				operator-() const							{ return Vector4(-x,-y,-z,-w); }

	Vector4				operator+(const Vector4& v) const			{ return Vector4(x+v.x, y+v.y, z+v.z, w+v.w); }
	Vector4&			operator+=(const Vector4& v)				{ x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }

	Vector4				operator-(const Vector4& v) const			{ return Vector4(x-v.x, y-v.y, z-v.z, w-v.w); }
	Vector4&			operator-=(const Vector4& v)				{ x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; }

	float				operator*(const Vector4& v) const			{ return x*v.x + y*v.y + z*v.z + w*v.w; }

	Vector4				operator*(const float f) const				{ return Vector4(x*f, y*f, z*f, w*f); }
	Vector4&			operator*=(const float f)					{ x*=f; y*=f; z*=f; w*=f; return *this; }
	
	Vector4				operator*(const Matrix4& m) const
	{
		return Vector4(x*m(0,0) + y*m(1,0) + z*m(2,0) + w*m(3,0),
					   x*m(0,1) + y*m(1,1) + z*m(2,1) + w*m(3,1),
					   x*m(0,2) + y*m(1,2) + z*m(2,2) + w*m(3,2),
					   x*m(0,3) + y*m(1,3) + z*m(2,3) + w*m(3,3));
	}

	Vector4&			operator*=(const Matrix4& m)
	{
		float tx=x;
		float ty=y;
		float tz=z;
		float tw=w;

		x=tx*m(0,0) + ty*m(1,0) + tz*m(2,0) + tw*m(3,0);
		y=tx*m(0,1) + ty*m(1,1) + tz*m(2,1) + tw*m(3,1);
		z=tx*m(0,2) + ty*m(1,2) + tz*m(2,2) + tw*m(3,2);
		w=tx*m(0,3) + ty*m(1,3) + tz*m(2,3) + tw*m(3,3);

		return *this;
	}

	Vector4				operator/(const float f) const				{ float inv=1.0f / f; return Vector4(x*inv, y*inv, z*inv, w*inv); }
	Vector4&			operator/=(const float f)					{ float inv=1.0f / f; x*=inv; y*=inv; z*=inv; w*=inv; return *this; }

	float				getSquareLength() const						{ return x*x + y*y + z*z + w*w; }
	float				getLength() const							{ return sqrtf(getSquareLength()); }
	void				normalize()
	{
		float fNorm=getSquareLength();
		if((FloatMakePositive(fNorm-1.0f) > SQREPSILON) && (fNorm > SQREPSILON))
		{
			float fInvNorm=FastInvSqrt(fNorm);
			x*=fInvNorm; y*=fInvNorm; z*=fInvNorm; w*=fInvNorm;
		}
	}

	void				negativeThis()								{ x=-x; y=-y; z=-z; w=-w; }

	float getDistanceFromSqr(const Vector4& v) const
	{
		float dx=x-v.x;
		float dy=y-v.y;
		float dz=z-v.z;
		float dw=w-v.w;
		return dx*dx+dy*dy+dz*dz+dw*dw;
	}
	float				getDistanceFrom(const Vector4& v) const		{ return sqrtf(getDistanceFromSqr(v)); }

	void				scale(const Vector4& v)						{ x*=v.x; y*=v.y; z*=v.z; w*=v.w; }
	void				scaleFrom(const Vector4& o, const Vector4& s)	{ x=o.x*s.x; y=o.y * s.y; z=o.z * s.z; w=o.w * s.w; }

	void				invScale(const Vector4& s)					{ x/=s.x; y/=s.y; z/=s.z; w/=s.w; }
	void				invScaleFrom(const Vector4& o, const Vector4& s)	{ x=o.x / s.x; y=o.y / s.y; z=o.z / s.z; w=o.w / s.w; }

	void				inverse()									{ x=1/x; y=1/y; z=1/z; w=1/w; }
	void				inverseFrom(const Vector4& v)				{ x=1/v.x; y=1/v.y; z=1/v.z; w=1/v.w; }
};

inline Vector4 operator*(float f, const Vector4& v)
{
	return v*f;
}

inline ostream& operator << (ostream& os, const Vector4& v)
{
	os << "< " << v.x << " , " << v.y << " , " << v.z << " , " << v.w << " >";
	return os;
}

}

#endif