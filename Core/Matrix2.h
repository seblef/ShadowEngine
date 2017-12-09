
#ifndef _MATRIX2_H_
#define _MATRIX2_H_

#include "MathUtils.h"
#include <ostream>

using namespace std;

namespace Core
{

class Matrix2
{
protected:

	float			m[2][2];

public:

	static Matrix2	Identity;

	static void		init()			{ Identity.createIdentity(); }

	Matrix2() {}
	Matrix2(const Matrix2& _m)
	{
		m[0][0]=_m.m[0][0]; m[0][1]=_m.m[0][1];
		m[1][0]=_m.m[1][0]; m[1][1]=_m.m[1][1];
	}
	Matrix2(const float *fp)
	{
		m[0][0]=fp[0]; m[0][1]=fp[1];
		m[1][0]=fp[2]; m[1][1]=fp[2];
	}
	~Matrix2() {}

	float			operator()(int i, int j) const				{ return m[i][j]; }
	float&			operator()(int i, int j)					{ return m[i][j]; }

	Matrix2&		operator=(const Matrix2& _m)
	{
		m[0][0]=_m.m[0][0]; m[0][1]=_m.m[0][1];
		m[1][0]=_m.m[1][0]; m[1][1]=_m.m[1][1];
		return *this;
	}

	Matrix2		operator*(const Matrix2& _m) const
	{
		Matrix2 temp;
		temp.m[0][0]=m[0][0]*_m.m[0][0] + m[0][1]*_m.m[1][0];
		temp.m[0][1]=m[0][0]*_m.m[0][1] + m[0][1]*_m.m[1][1];
		temp.m[1][0]=m[1][0]*_m.m[0][0] + m[1][1]*_m.m[1][0];
		temp.m[1][1]=m[1][0]*_m.m[0][1] + m[1][1]*_m.m[1][1];
		return temp;
	}

	Matrix2&		operator*=(const Matrix2& _m)
	{
		Matrix2 temp;
		temp.m[0][0]=m[0][0]*_m.m[0][0] + m[0][1]*_m.m[1][0];
		temp.m[0][1]=m[0][0]*_m.m[0][1] + m[0][1]*_m.m[1][1];
		temp.m[1][0]=m[1][0]*_m.m[0][0] + m[1][1]*_m.m[1][0];
		temp.m[1][1]=m[1][0]*_m.m[0][1] + m[1][1]*_m.m[1][1];

		return (*this)=temp;
	}

	void			createIdentity()
	{
		m[0][0]=1.0f; m[0][1]=0.0f;
		m[1][0]=0.0f; m[1][1]=1.0f;
	}

	void			scale(float f)
	{
		m[0][0]=f; m[0][1]=0.0f;
		m[1][0]=0.0f; m[1][1]=f;
	}

	void			scale(float fx, float fy)
	{
		m[0][0]=fx; m[0][1]=0.0f;
		m[1][0]=0.0f; m[1][1]=fy;
	}

	void			rotation(float angle)
	{
		float c=cosf(angle);
		float s=sinf(angle);

		m[0][0]= c; m[0][1]=s;
		m[1][0]=-s; m[1][1]=c;
	}

	void			rotationScale(float angle, float scale)
	{

		float c=cosf(angle) * scale;
		float s=sinf(angle) * scale;

		m[0][0]= c; m[0][1]=s;
		m[1][0]=-s; m[1][1]=c;
	}

	void			rotationScale(float angle, float xscale, float yscale)
	{
		float c=cosf(angle) * xscale;
		float s=sinf(angle) * yscale;

		m[0][0]= c; m[0][1]=s;
		m[1][0]=-s; m[1][1]=c;
	}

	bool			inverse(Matrix2& _m) const
	{
		float d=(m[0][0] * m[1][1] - m[1][0] * m[0][1]);
		if(d==0.0f) return false;

		d=1.0f / d;

		_m.m[0][0]=d*m[1][1];
		_m.m[1][0]=-d*m[1][0];
		_m.m[0][1]=-d*m[0][1];
		_m.m[1][1]=d*m[0][0];
	}
};

inline ostream& operator << (ostream& os, const Matrix2& m)
{
	for(int i=0;i<2;++i)
		os << "| " << m(i,0) << " , " << m(i,1) << " |" << endl;
	return os;
}

}

#endif