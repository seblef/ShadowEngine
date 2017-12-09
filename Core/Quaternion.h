
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Vector3.h"
#include "MathUtils.h"
#include "CoreCommon.h"

namespace Core
{

class Quaternion
{
	public:

		float				x,y,z,w;

		Quaternion() : x(0.0f),y(0.0f),z(0.0f),w(1.0f) {}
		Quaternion(float _x, float _y, float _z, float _w) : x(_x),y(_y),z(_z),w(_w) {}
		Quaternion(const Quaternion& quat) : x(quat.x),y(quat.y),z(quat.z),w(quat.w) {}
		Quaternion(const Matrix4& mat)									{ buildMatrix(mat); }
		Quaternion(const Matrix3& mat)									{ buildMatrix(mat); }
		Quaternion(const Vector3& vAxis, const float fAngle)			{ buildAxisAngle(vAxis,fAngle); }
		~Quaternion() {}

		void				buildMatrix(const Matrix4& mat);
		void				buildMatrix(const Matrix3& mat);
		void				buildAxisAngle(const Vector3& axis, float fAngle);
		void				buildEuler(const Vector3& vRot)				{ buildEuler(vRot.x,vRot.y,vRot.z); }
		void				buildEuler(float ex, float ey, float ez);
		void				buildRotationArc(const Vector3& vFrom, const Vector3& vTo);

		void				getMatrix(Matrix4& mat) const;
		void				getMatrix(Matrix3& mat) const;
		void				getAxisAngle(Vector3& vAxis, float &fAngle) const;
		Quaternion			getInverse() const							{ return Quaternion(-x,-y,-z,w); }
		void				getInverse(Quaternion& q) const				{ q.x=-x; q.y=-y; q.z=-z; q.w=w; }

		void				normalize()
		{
			float fNorm=x*x + y*y + z*z + w*w;
			if(fNorm < SEPSILON)
			{
				x=y=z=0.0f;
				w=1.0f; 
			}
			else if(FloatMakePositive(fNorm - 1.0f) > SEPSILON)
			{
				const float fScale=FastInvSqrt(fNorm);
				x*=fScale; y*=fScale; z*=fScale; w*=fScale;
			}
		}
		void				reset() { x=y=z=0.0f; w=1.0f; }
		void				inverse() { x=-x; y=-y; z=-z; }

		Vector3				operator*(const Vector3& v) const
		{
			Vector3 v0(x,y,z);
			Vector3 v1(v0 ^ v);
			Vector3 v2(v0 ^ v1);
			v1*=(2.0f*w);
			v2*=2.0f;

			return v+v1+v2;
		}

		bool				operator==(const Quaternion& quat) const	{	return (x==quat.x) && (y==quat.y) && (z==quat.z) && (w==quat.w); }
		bool				operator!=(const Quaternion& quat) const	{	return (x!=quat.x) || (y!=quat.y) || (z!=quat.z) || (w!=quat.w); }

		Quaternion&			operator=(const Quaternion& quat)			{ x=quat.x; y=quat.y; z=quat.z; w=quat.w; return *this; }

		static void			align(const Quaternion& q1, const Quaternion& q2, Quaternion& qRet)
		{
			float ta=q1.x- q2.x;
			float tb=q1.x+ q2.x;

			float a=ta*ta;
			float b=tb*tb;

			ta=q1.y-q2.y; tb=q1.y+q2.y;
			a+=ta*ta; b+=tb*tb;

			ta=q1.z-q2.z; tb=q1.z+q2.z;
			a+=ta*ta; b+=tb*tb;

			ta=q1.w-q2.w; tb=q1.w+q2.w;
			a+=ta*ta; b+=tb*tb;

			if(a > b)
			{
				qRet.x=-q2.x;
				qRet.y=-q2.y;
				qRet.z=-q2.z;
				qRet.w=-q2.w;
			}
			else if(&qRet!=&q1)
				qRet=q2;
		}

		static void			mult(const Quaternion& q1, const Quaternion& q2, Quaternion& qRet)
		{
			Quaternion qTemp;
			align(q1,q2,qTemp);

			qRet.x= q1.x * qTemp.w + q1.y * qTemp.z - q1.z * qTemp.y + q1.w * qTemp.x;
			qRet.y=-q1.x * qTemp.z + q1.y * qTemp.w + q1.z * qTemp.x + q1.w * qTemp.y;
			qRet.z= q1.x * qTemp.y - q1.y * qTemp.x + q1.z * qTemp.w + q1.w * qTemp.z;
			qRet.w=-q1.x * qTemp.x - q1.y * qTemp.y - q1.z * qTemp.z + q1.w * qTemp.w;
		}

		static void			slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& qRet)
		{
			Quaternion qTemp;
			align(q1,q2,qTemp);

			float fSclQ1, fSclQ2;

			const float fCos=q1.x * qTemp.x + q1.y * qTemp.y + q1.z * qTemp.z + q1.w * qTemp.w;

			if((1.0f + fCos) > 0.0000001f)
			{
				if((1.0f - fCos) > 0.000001f)
				{
					const float fOmega=acosf(fCos);
					const float fSin=1.0f / sinf(fOmega);
					fSclQ1=sinf((1.0f - t) * fOmega) * fSin;
					fSclQ2=sinf(t*fOmega) * fSin;
				}
				else
				{
					fSclQ1=1.0f -t;
					fSclQ2=t;
				}

				qRet.x=fSclQ1 * q1.x + fSclQ2 * qTemp.x;
				qRet.y=fSclQ1 * q1.y + fSclQ2 * qTemp.y;
				qRet.z=fSclQ1 * q1.z + fSclQ2 * qTemp.z;
				qRet.w=fSclQ1 * q1.w + fSclQ2 * qTemp.w;
			}
			else
			{
				qRet.x=-q1.y;
				qRet.y= q1.x;
				qRet.z=-q1.w;
				qRet.w= q1.z;

				fSclQ1=sinf((1.0f - t) * (0.5f * M_PI));
				fSclQ2=sinf(t * (0.5f * M_PI));

				qRet.x=fSclQ1 * q1.x + fSclQ2 * qTemp.x;
				qRet.y=fSclQ1 * q1.y + fSclQ2 * qTemp.y;
				qRet.z=fSclQ1 * q1.z + fSclQ2 * qTemp.z;
				qRet.w=fSclQ1 * q1.w + fSclQ2 * qTemp.w;
			}

			qRet.normalize();
		}
};

inline Vector3 operator*(const Vector3& v, const Quaternion& q)
{
	return q*v;
}

}

#endif
