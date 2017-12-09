
#ifndef _SOUNDUTIL_H_
#define _SOUNDUTIL_H_

#include <Core.h>
#include <X3DAudio.h>

using namespace Core;

inline void soundCopyVec(const Vector3& s, X3DAUDIO_VECTOR& x)
{
	x.x = s.x;
	x.y = s.y;
	x.z = s.z;
}

inline void soundCopyVec(const X3DAUDIO_VECTOR& x, Vector3& s)
{
	s.x = x.x;
	s.y = x.y;
	s.z = x.z;
}

#define SOUND_RETURN_VEC(vec)		*((Vector3*)&vec)
#define SOUND_RETURN_VEC_CONST(vec)	*((const Vector3*)&vec)

#endif