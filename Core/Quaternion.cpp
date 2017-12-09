
#include "Quaternion.h"

using namespace Core;

void Quaternion::buildMatrix(const Matrix4& mat)
{
	float fTrace=mat(0,0) + mat(1,1) + mat(2,2);
	float fRoot;
	if(fTrace > 0.0f)
	{
		fRoot=sqrtf(fTrace + 1.0f);
		w=0.5f*fRoot;
		fRoot=0.5f / fRoot;
		x=(mat(2,1) - mat(1,2))*fRoot;
		y=(mat(0,2) - mat(2,0))*fRoot;
		z=(mat(1,0) - mat(0,1))*fRoot;
	}
	else
	{
		int iNext[3]={1,2,0};
		int i=0;
		if(mat(1,1) > mat(0,0))
			i=1;

		if(mat(2,2) > mat(i,i))
			i=2;

		int j=iNext[i]; 
		int k=iNext[j];
		fRoot=sqrtf(mat(i,i) - mat(j,j) - mat(k,k) + 1.0f);
		float *pQuat[3]={&x,&y,&z};
		*pQuat[i]=0.5f*fRoot;

		fRoot=0.5f/fRoot;
		w=(mat(k,j) - mat(j,k))*fRoot;
		*(pQuat[j])=(mat(j,i) + mat(i,j))*fRoot;
		*(pQuat[k])=(mat(k,i) + mat(i,k))*fRoot;
	}

	normalize();
}

void Quaternion::buildMatrix(const Matrix3& mat)
{
	float fTrace=mat(0,0) + mat(1,1) + mat(2,2);
	float fRoot;
	if(fTrace > 0.0f)
	{
		fRoot=sqrtf(fTrace + 1.0f);
		w=0.5f*fRoot;
		fRoot=0.5f / fRoot;
		x=(mat(2,1) - mat(1,2))*fRoot;
		y=(mat(0,2) - mat(2,0))*fRoot;
		z=(mat(1,0) - mat(0,1))*fRoot;
	}
	else
	{
		int iNext[3]={1,2,0};
		int i=0;
		if(mat(1,1) > mat(0,0))
			i=1;

		if(mat(2,2) > mat(i,i))
			i=2;

		int j=iNext[i]; 
		int k=iNext[j];
		fRoot=sqrtf(mat(i,i) - mat(j,j) - mat(k,k) + 1.0f);
		float *pQuat[3]={&x,&y,&z};
		*pQuat[i]=0.5f*fRoot;

		fRoot=0.5f/fRoot;
		w=(mat(k,j) - mat(j,k))*fRoot;
		*(pQuat[j])=(mat(j,i) + mat(i,j))*fRoot;
		*(pQuat[k])=(mat(k,i) + mat(i,k))*fRoot;
	}

	normalize();
}

void Quaternion::buildAxisAngle(const Vector3& vAxis, float fAngle)
{
	float fHalfAngle=fAngle*0.5f;
	float fSin=sinf(fHalfAngle);
	x=fSin*vAxis.x;
	y=fSin*vAxis.y;
	z=fSin*vAxis.z;
	w=cosf(fHalfAngle);

	normalize();
}

void Quaternion::buildEuler(float ex, float ey, float ez)
{
	float fAngle,sr,sp,sy,cr,cp,cy;
	fAngle=ez*0.5f; sy=sinf(fAngle); cy=cosf(fAngle);
	fAngle=ey*0.5f; sp=sinf(fAngle); cp=cosf(fAngle);
	fAngle=ex*0.5f; sr=sinf(fAngle); cr=cosf(fAngle);

	float cpcy=cp*cy;
	float spcy=sp*cy;
	float cpsy=cp*sy;
	float spsy=sp*sy;

	x=sr * cpcy - cr * spsy;
	y=cr * spcy + sr * cpsy;
	z=cr * cpsy - sr * spcy;
	w=cr * cpcy + sr * spsy;

	Quaternion qX(Vector3::XAxisVector,ex);
	Quaternion qY(Vector3::YAxisVector,ey);
	Quaternion qZ(Vector3::ZAxisVector,ez);

	mult(qY,qX,*this);
	mult(qZ,*this,*this);

	normalize();
}

void Quaternion::buildRotationArc(const Vector3& vFrom, const Vector3& vTo)
{
	Vector3 v0(vFrom);
	Vector3 v1(vTo);

	v0.normalize();
	v1.normalize();

	Vector3 c(v0^v1);
	float d=v0*v1;

	if(d>=1.0f)
		reset();
	else
	{
		float invs=FastInvSqrt((1.0f + d)*2.0f);
		float s=1.0f / invs;

		x=c.x * invs;
		y=c.y * invs;
		z=c.z * invs;
		w=s*0.5f;
	}
}

void Quaternion::getMatrix(Matrix4& mat) const
{
	float fTX=2.0f*x, fTY=2.0f*y, fTZ=2.0f*z;
	float fTWX=fTX*w, fTWY=fTY*w, fTWZ=fTZ*w;
	float fTXX=fTX*x, fTXY=fTY*x, fTXZ=fTZ*x;
	float fTYY=fTY*y, fTYZ=fTZ*y;
	float fTZZ=fTZ*z;
	mat(0,0)=1.0f - (fTYY+fTZZ); mat(0,1)=fTXY - fTWZ;		mat(0,2)=fTXZ + fTWY;		mat(0,3)=0.0f;
	mat(1,0)=fTXY + fTWZ;		mat(1,1)=1.0f - (fTXX+fTZZ); mat(1,2)=fTYZ - fTWX;		mat(1,3)=0.0f;
	mat(2,0)=fTXZ - fTWY;		mat(2,1)=fTYZ + fTWX;		mat(2,2)=1.0f - (fTXX+fTYY); mat(2,3)=0.0f;
	mat(3,0)=0.0f;				mat(3,1)=0.0f;				mat(3,2)=0.0f;				mat(3,3)=1.0f;
}

void Quaternion::getMatrix(Matrix3& mat) const
{
	float fTX=2.0f*x, fTY=2.0f*y, fTZ=2.0f*z;
	float fTWX=fTX*w, fTWY=fTY*w, fTWZ=fTZ*w;
	float fTXX=fTX*x, fTXY=fTY*x, fTXZ=fTZ*x;
	float fTYY=fTY*y, fTYZ=fTZ*y;
	float fTZZ=fTZ*z;
	mat(0,0)=1.0f - (fTYY+fTZZ); mat(0,1)=fTXY - fTWZ;		mat(0,2)=fTXZ + fTWY;
	mat(1,0)=fTXY + fTWZ;		mat(1,1)=1.0f - (fTXX+fTZZ); mat(1,2)=fTYZ - fTWX;
	mat(2,0)=fTXZ - fTWY;		mat(2,1)=fTYZ + fTWX;		mat(2,2)=1.0f - (fTXX+fTYY);
}

void Quaternion::getAxisAngle(Vector3& vAxis, float &fAngle) const
{
	float fSqrLen=x*x+y*y+z*z;
	if(fSqrLen > 0.0f)
	{
		fAngle=2.0f * acosf(w);
		float fInvLen=1.0f / logf(fSqrLen);
		vAxis.x=x*fInvLen; vAxis.y=y*fInvLen; vAxis.z=z*fInvLen;
	}
	else
	{
		fAngle=0.0f;
		vAxis.x=1.0f; vAxis.y=0.0f; vAxis.z=0.0f;
	}
}
