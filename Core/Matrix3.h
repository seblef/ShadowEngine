
#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include "Matrix2.h"
#include <memory.h>

namespace Core
{

class Matrix3
{
protected:
	
	float			m[3][3];

public:

	static Matrix3	Identity;

	static void		init()							{ Identity.createIdentity(); }

	Matrix3()		{}
	~Matrix3()		{}

	Matrix3(const Matrix3& _m)						{ memcpy((void *)m,(void *)_m.m,9*sizeof(float)); }
	Matrix3(const float *fp)						{ memcpy((void *)m,fp,9*sizeof(float)); }

	Matrix3&		operator=(const Matrix3& _m)			{ memcpy((void *)m,(void *)_m.m,9*sizeof(float)); return *this; }

	Matrix3			operator*(const Matrix3& _m) const
	{
		Matrix3 temp;
		int i,j;

		for(i=0;i<3;++i)
			for(j=0;j<3;++j)
				temp.m[i][j]=m[i][0] * _m.m[0][j] +
							 m[i][1] * _m.m[1][j] +
							 m[i][2] * _m.m[2][j];

		return temp;
	}

	Matrix3&		operator*=(const Matrix3& _m)
	{
		Matrix3 temp;
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				temp.m[i][j]=m[i][0] * _m.m[0][j] +
								m[i][1] * _m.m[1][j] +
								m[i][2] * _m.m[2][j];

		return (*this)=temp;
	}

	Matrix3&		operator*=(float f)
	{
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				m[i][j]*=f;

		return *this;
	}
	Matrix3&		operator+=(const Matrix3& _m)
	{
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				m[i][j]+=_m.m[i][j];

		return *this;
	}
	Matrix3&		operator-=(const Matrix3& _m)
	{
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				m[i][j]-=_m.m[i][j];

		return *this;
	}

	float			operator()(int i, int j) const				{ return m[i][j]; }
	float&			operator()(int i, int j)					{ return m[i][j]; }

	void			createIdentity()
	{
		m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f;
		m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f;
	}

	void			createScale(float x, float y, float z)
	{
		m[0][0]=x   ; m[0][1]=0.0f; m[0][2]=0.0f;
		m[1][0]=0.0f; m[1][1]=y   ; m[1][2]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=z   ;
	}
		
	void			createRotateX(float x)
	{
		float c=cosf(x);
		float s=sinf(x);

		m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f;
		m[1][0]=0.0f; m[1][1]=c;   m[1][2]=s;
		m[2][0]=0.0f; m[2][1]=-s;  m[2][2]=c;
	}

	void			createRotateY(float y)
	{
		float c=cosf(y);
		float s=sinf(y);

		m[0][0]=c;   m[0][1]=0.0f; m[0][2]=-s;
		m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f;
		m[2][0]=s;   m[2][1]=0.0f; m[2][2]=c;
	}

	void			createRotateZ(float z)
	{
		float c=cosf(z);
		float s=sinf(z);

		m[0][0]=c;   m[0][1]=s;   m[0][2]=0.0f;
		m[1][0]=-s;  m[1][1]=c;   m[1][2]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f;
	}

	void			createRotateXYZ(float x, float y, float z)
	{
		float cr=cosf(x), sr=sinf(z);
		float cp=cosf(y), sp=sinf(y);
		float cy=cosf(z), sy=sinf(z);
		float srsp=sr*sp, crsp=cr*sp;

		m[0][0]=cp*cy; m[0][1]=cp*sy; m[0][2]=-sp;
		m[1][0]=srsp*cy-cr*sy;
		m[1][1]=srsp*sy+cr*cy;
		m[1][2]=sr*cp;

		m[2][0]=crsp*cy+sr*sy;
		m[2][1]=crsp*sy-sr*cy;
		m[2][2]=cr*cp;
	}

	void			transpose(Matrix3& mat) const
	{
		mat.m[0][0]=m[0][0]; mat.m[0][1]=m[1][0]; mat.m[0][2]=m[2][0];
		mat.m[1][0]=m[0][1]; mat.m[1][1]=m[1][1]; mat.m[1][2]=m[2][1];
		mat.m[2][0]=m[0][2]; mat.m[2][1]=m[1][2]; mat.m[2][2]=m[2][2];
	}
};

inline ostream& operator << (ostream& os, const Matrix3& m)
{
	for(int i=0;i<3;++i)
		os << "| " << m(i,0) << " , " << m(i,1) << " , " << m(i,2) << " |" << endl;

	return os;
}

}

#endif
