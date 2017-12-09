
#include "stdafx.h"

namespace Core
{

BBox2	BBox2::NullBox(0,0,0,0);
BBox2	BBox2::BoundBox(WBOUNDS_N,WBOUNDS_N,WBOUNDS_P,WBOUNDS_P);

BBox3	BBox3::NullBox(0,0,0,0,0,0);
BBox3	BBox3::BoundBox(WBOUNDS_N,WBOUNDS_N,WBOUNDS_N,WBOUNDS_P,WBOUNDS_P,WBOUNDS_P);
BBox3	BBox3::InitBox(WBOUNDS_P, WBOUNDS_P, WBOUNDS_P, WBOUNDS_N, WBOUNDS_N, WBOUNDS_N);

bool BBox3::rayIntersect(const Vector3& o, const Vector3& d) const
{
	Vector3 vAbsSegDir,c,s;
	float f1,f2;

	getCenter(c);
	getSize(s);

	const Vector3 vDiff(o-c);

	if(vDiff.x*d.x >= 0.0f && FloatMakePositive(vDiff.x) > s.x) return false;
	if(vDiff.y*d.y >= 0.0f && FloatMakePositive(vDiff.y) > s.y) return false;
	if(vDiff.z*d.z >= 0.0f && FloatMakePositive(vDiff.z) > s.z) return false;

	vAbsSegDir.x=FloatMakePositive(d.x);
	vAbsSegDir.y=FloatMakePositive(d.y);
	vAbsSegDir.z=FloatMakePositive(d.z);

	f1=FloatMakePositive(d.y * vDiff.z - d.z * vDiff.y);
	f2=s.y * vAbsSegDir.z + s.z * vAbsSegDir.y;
	if(f1 > f2) return false;

	f1=FloatMakePositive(d.z * vDiff.x - d.x * vDiff.z);
	f2=s.x * vAbsSegDir.z + s.z * vAbsSegDir.x;
	if(f1 > f2) return false;

	f1=FloatMakePositive(d.x * vDiff.y - d.y * vDiff.x);
	f2=s.x * vAbsSegDir.y + s.y * vAbsSegDir.x;
	if(f1 > f2) return false;

	return true;
}

Matrix2 Matrix2::Identity;
Matrix4 Matrix4::Identity;

bool Matrix4::inverse(Matrix4& mat) const
{
	float d=(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - (m[0][0] * m[2][1] - m[2][0] * m[0][1]) * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) +
			(m[0][0] * m[3][1] - m[3][0] * m[0][1]) * (m[1][2] * m[2][3] - m[2][2] * m[1][3]) + (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) -
			(m[1][0] * m[3][1] - m[3][0] * m[1][1]) * (m[0][2] * m[2][3] - m[2][2] * m[0][3]) + (m[2][0] * m[3][1] - m[3][0] * m[2][1]) * (m[0][2] * m[1][3] - m[1][2] * m[0][3]);

	if(d==0.0f)
		return false;

	d=1.0f / d;

	mat.m[0][0]=d * (m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) + m[2][1] * (m[3][2] * m[1][3] - m[1][2] * m[3][3]) + m[3][1] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]));
	mat.m[1][0]=d * (m[1][2] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) + m[2][2] * (m[3][0] * m[1][3] - m[1][0] * m[3][3]) + m[3][2] * (m[1][0] * m[2][3] - m[2][0] * m[1][3]));
	mat.m[2][0]=d * (m[1][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]) + m[2][3] * (m[3][0] * m[1][1] - m[1][0] * m[3][1]) + m[3][3] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]));
	mat.m[3][0]=d * (m[1][0] * (m[3][1] * m[2][2] - m[2][1] * m[3][2]) + m[2][0] * (m[1][1] * m[3][2] - m[3][1] * m[1][2]) + m[3][0] * (m[2][1] * m[1][2] - m[1][1] * m[2][2]));

	mat.m[0][1]=d * (m[2][1] * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) + m[3][1] * (m[2][2] * m[0][3] - m[0][2] * m[2][3]) + m[0][1] * (m[3][2] * m[2][3] - m[2][2] * m[3][3]));
	mat.m[1][1]=d * (m[2][2] * (m[0][0] * m[3][3] - m[3][0] * m[0][3]) + m[3][2] * (m[2][0] * m[0][3] - m[0][0] * m[2][3]) + m[0][2] * (m[3][0] * m[2][3] - m[2][0] * m[3][3]));
	mat.m[2][1]=d * (m[2][3] * (m[0][0] * m[3][1] - m[3][0] * m[0][1]) + m[3][3] * (m[2][0] * m[0][1] - m[0][0] * m[2][1]) + m[0][3] * (m[3][0] * m[2][1] - m[2][0] * m[3][1]));
	mat.m[3][1]=d * (m[2][0] * (m[3][1] * m[0][2] - m[0][1] * m[3][2]) + m[3][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) + m[0][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]));

	mat.m[0][2]=d * (m[3][1] * (m[0][2] * m[1][3] - m[1][2] * m[0][3]) + m[0][1] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) + m[1][1] * (m[3][2] * m[0][3] - m[0][2] * m[3][3]));
	mat.m[1][2]=d * (m[3][2] * (m[0][0] * m[1][3] - m[1][0] * m[0][3]) + m[0][2] * (m[1][0] * m[3][3] - m[3][0] * m[1][3]) + m[1][2] * (m[3][0] * m[0][3] - m[0][0] * m[3][3]));
	mat.m[2][2]=d * (m[3][3] * (m[0][0] * m[1][1] - m[1][0] * m[0][1]) + m[0][3] * (m[1][0] * m[3][1] - m[3][0] * m[1][1]) + m[1][3] * (m[3][0] * m[0][1] - m[0][0] * m[3][1]));
	mat.m[3][2]=d * (m[3][0] * (m[1][1] * m[0][2] - m[0][1] * m[1][2]) + m[0][0] * (m[3][1] * m[1][2] - m[1][1] * m[3][2]) + m[1][0] * (m[0][1] * m[3][2] - m[3][1] * m[0][2]));

	mat.m[0][3]=d * (m[0][1] * (m[2][2] * m[1][3] - m[1][2] * m[2][3]) + m[1][1] * (m[0][2] * m[2][3] - m[2][2] * m[0][3]) + m[2][1] * (m[1][2] * m[0][3] - m[0][2] * m[1][3]));
	mat.m[1][3]=d * (m[0][2] * (m[2][0] * m[1][3] - m[1][0] * m[2][3]) + m[1][2] * (m[0][0] * m[2][3] - m[2][0] * m[0][3]) + m[2][2] * (m[1][0] * m[0][3] - m[0][0] * m[1][3]));
	mat.m[2][3]=d * (m[0][3] * (m[2][0] * m[1][1] - m[1][0] * m[2][1]) + m[1][3] * (m[0][0] * m[2][1] - m[2][0] * m[0][1]) + m[2][3] * (m[1][0] * m[0][1] - m[0][0] * m[1][1]));
	mat.m[3][3]=d * (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) + m[1][0] * (m[2][1] * m[0][2] - m[0][1] * m[2][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

	return true;
}

void Matrix4::lookAtLH(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Vector3 z(target - pos);
	z.normalize();

	Vector3 x(up ^ z);
	x.normalize();

	Vector3 y(z ^ x);

	float dx=-(x * pos);
	float dy=-(y * pos);
	float dz=-(z * pos);

	m[0][0]=x.x;	m[0][1]=y.x;	m[0][2]=z.x;	m[0][3]=0.0f;
	m[1][0]=x.y;	m[1][1]=y.y;	m[1][2]=z.y;	m[1][3]=0.0f;
	m[2][0]=x.z;	m[2][1]=y.z;	m[2][2]=z.z;	m[2][3]=0.0f;
	m[3][0]=dx;		m[3][1]=dy;		m[3][2]=dz;		m[3][3]=1.0f;
}

void Matrix4::orthoLH(float width, float height, float znear, float zfar)
{
	createIdentity();
	float invz=1.0f / (zfar - znear);

	m[0][0]=2.0f / width;
	m[1][1]=2.0f / height;
	m[2][2]=invz;
	m[3][2]=znear * invz;
}

void Matrix4::perspectiveFOVLH(float fov, float aspect, float znear, float zfar)
{
	createIdentity();
	float h=1.0f / tanf(fov * 0.5f);
	float w=h / aspect;
	float invz=zfar / (zfar - znear);

	m[0][0]=w;
	m[1][1]=h;
	m[2][2]=invz;
	m[3][2]=-znear * invz;
	m[2][3]=1.0f;
	m[3][3]=0.0f;
}

Matrix4& Matrix4::operator=(const Vector3& v)
{
	m[3][0]=v.x;
	m[3][1]=v.y;
	m[3][2]=v.z;

	return *this;
}

Matrix4& Matrix4::operator=(const Vector4& v)
{
	m[3][0]=v.x;
	m[3][1]=v.y;
	m[3][2]=v.z;
	m[3][3]=v.w;

	return *this;
}

};