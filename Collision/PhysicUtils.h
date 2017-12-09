
#ifndef _PHYSICUTIL_H_
#define _PHYSICUTIL_H_

#define PMAKETRANSFORM(t, m)					PxTransform t(*((PxMat44*)&m)); if(!t.isValid()) t.q.normalize();

#define PXVEC(v)								*((PxVec3*)&v)
#define PXVEC_C(v)								*((const PxVec3*)&v)

#define VEC3(v)									*((Vector3*)&v)
#define VEC3_C(v)								*((const Vector3*)&v)

#endif