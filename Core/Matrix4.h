
#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "Matrix3.h"
#include <memory.h>

namespace Core
{

class Vector3;
class Vector4;

class Matrix4
{
protected:
	
	float			m[4][4];

public:

	static Matrix4	Identity;

	static void		init()							{ Identity.createIdentity(); }

	Matrix4()		{}
	~Matrix4()		{}

	Matrix4(const Matrix4& _m)						{ memcpy((void *)m,(void *)_m.m,16*sizeof(float)); }
	Matrix4(const Matrix3& _m)
	{
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				m[i][j]=_m(i,j);
	}
	Matrix4(const float *fp)						{ memcpy((void *)m,fp,16*sizeof(float)); }

	Matrix4&		operator=(const Matrix4& _m)			{ memcpy((void *)m,(void *)_m.m,16*sizeof(float)); return *this; }
	Matrix4&		operator=(const Matrix3& _m)
	{
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				m[i][j]=_m(i,j);
		return *this;
	}

	Matrix4			operator*(const Matrix4& _m) const
	{
		Matrix4 temp;
		int i,j;

		for(i=0;i<4;++i)
			for(j=0;j<4;++j)
				temp.m[i][j]=m[i][0] * _m.m[0][j] +
							 m[i][1] * _m.m[1][j] +
							 m[i][2] * _m.m[2][j] +
							 m[i][3] * _m.m[3][j];

		return temp;
	}

	Matrix4&		operator*=(const Matrix4& _m)
	{
		Matrix4 temp;
		int i,j;

		for(i=0;i<4;++i)
			for(j=0;j<4;++j)
				temp.m[i][j]=m[i][0] * _m.m[0][j] +
								m[i][1] * _m.m[1][j] +
								m[i][2] * _m.m[2][j] +
								m[i][3] * _m.m[3][j];

		return (*this)=temp;
	}

	float			operator()(int i, int j) const				{ return m[i][j]; }
	float&			operator()(int i, int j)					{ return m[i][j]; }

	void			createIdentity()
	{
		m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
		m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f; m[1][3]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; m[2][3]=0.0f;
		m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
	}

	void			createTranslate(float x, float y, float z)
	{
		m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
		m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f; m[1][3]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; m[2][3]=0.0f;
		m[3][0]=x   ; m[3][1]=y   ; m[3][2]=z   ; m[3][3]=1.0f;
	}

	void			createScale(float x, float y, float z)
	{
		m[0][0]=x   ; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
		m[1][0]=0.0f; m[1][1]=y   ; m[1][2]=0.0f; m[1][3]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=z   ; m[2][3]=0.0f;
		m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
	}
		
	void			createRotateX(float x)
	{
		float c=cosf(x);
		float s=sinf(x);

		m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
		m[1][0]=0.0f; m[1][1]=c;   m[1][2]=s;   m[1][3]=0.0f;
		m[2][0]=0.0f; m[2][1]=-s;  m[2][2]=c;   m[2][3]=0.0f;
		m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
	}

	void			createRotateY(float y)
	{
		float c=cosf(y);
		float s=sinf(y);

		m[0][0]=c;   m[0][1]=0.0f; m[0][2]=-s;  m[0][3]=0.0f;
		m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f; m[1][3]=0.0f;
		m[2][0]=s;   m[2][1]=0.0f; m[2][2]=c;   m[2][3]=0.0f;
		m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
	}

	void			createRotateZ(float z)
	{
		float c=cosf(z);
		float s=sinf(z);

		m[0][0]=c;   m[0][1]=s;   m[0][2]=0.0f; m[0][3]=0.0f;
		m[1][0]=-s;  m[1][1]=c;   m[1][2]=0.0f; m[1][3]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; m[2][3]=0.0f;
		m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
	}

	void			createRotateXYZ(float x, float y, float z)
	{
		float cr=cosf(x), sr=sinf(z);
		float cp=cosf(y), sp=sinf(y);
		float cy=cosf(z), sy=sinf(z);
		float srsp=sr*sp, crsp=cr*sp;

		m[0][0]=cp*cy; m[0][1]=cp*sy; m[0][2]=-sp; m[0][3]=0.0f;
		m[1][0]=srsp*cy-cr*sy;
		m[1][1]=srsp*sy+cr*cy;
		m[1][2]=sr*cp; m[1][3]=0.0f;

		m[2][0]=crsp*cy+sr*sy;
		m[2][1]=crsp*sy-sr*cy;
		m[2][2]=cr*cp; m[2][3]=0.0f;

		m[3][0]=m[3][1]=m[3][2]=0.0f; m[3][3]=1.0f;
	}

	bool			inverse(Matrix4& mat) const;

	void			transpose(Matrix4& mat) const
	{
		mat.m[0][0]=m[0][0]; mat.m[0][1]=m[1][0]; mat.m[0][2]=m[2][0]; mat.m[0][3]=m[3][0];
		mat.m[1][0]=m[0][1]; mat.m[1][1]=m[1][1]; mat.m[1][2]=m[2][1]; mat.m[1][3]=m[3][1];
		mat.m[2][0]=m[0][2]; mat.m[2][1]=m[1][2]; mat.m[2][2]=m[2][2]; mat.m[2][3]=m[3][2];
		mat.m[3][0]=m[0][3]; mat.m[3][1]=m[1][3]; mat.m[3][2]=m[2][3]; mat.m[3][3]=m[3][3];
	}

	void			lookAtLH(const Vector3& pos, const Vector3& target, const Vector3& up);
	void			orthoLH(float width, float height, float near, float far);
	void			perspectiveFOVLH(float fov, float aspect, float near, float far);

	void			getMatrix3(Matrix3& _m) const
	{
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				_m(i,j)=m[i][j];
	}
					operator Matrix3() const
	{
		Matrix3 r;
		getMatrix3(r);
		return r;
	}

	void			reinitTranslation()
	{
		m[3][0]=m[3][1]=m[3][2]=0;
	}
	void			reinitRotationScale()
	{
		m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f;
		m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f;
		m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f;
	}


	Matrix4&	operator=(const Vector3& v);
	Matrix4&	operator=(const Vector4& v);
				operator Vector3&()				{ return *((Vector3*)&m[3][0]); }
				operator Vector4&()				{ return *((Vector4*)&m[3][0]); }
				operator const Vector3&()		{ return *((Vector3*)&m[3][0]); }
				operator const Vector4&()		{ return *((Vector4*)&m[3][0]); }
};

inline ostream& operator << (ostream& os, const Matrix4& m)
{
	for(int i=0;i<4;++i)
		os << "| " << m(i,0) << " , " << m(i,1) << " , " << m(i,2) << " , " << m(i,3) << " |" << endl;

	return os;
}

}

#endif
