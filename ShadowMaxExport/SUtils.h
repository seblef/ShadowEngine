
#ifndef _SUTILS_H_
#define _SUTILS_H_

#include <Core.h>
#include <max.h>

inline void copyP3(const Point3& p, Core::Vector3& v)
{
	v.x=p.x;
	v.y=p.y;
	v.z=p.z;
}

inline void copyP2(const Point2& p, Core::Vector2& v)
{
	v.x=p.x;
	v.y=p.y;
}

inline void copyMatrix(const GMatrix& mSrc, Core::Matrix4& mDest)
{
	memcpy((void*)&mDest,mSrc.GetAddr(),16*sizeof(float));
}

void validMatrix(Core::Matrix4& m);
void offsetMatrix(IGameNode *pNode, Core::Matrix4& mSOffset, Core::Matrix4& mSOffsetNoRot);

#endif