
#ifndef _RAYCASTINFOS_H_
#define _RAYCASTINFOS_H_

#include "PhysicDefs.h"


class RayCastInfos
{
public:

	Vector3					_origin;
	Vector3					_dir;
	float					_range;
	unsigned int			_flags;

	Vector3					_impact;
	Vector3					_normal;

	RayCastInfos() : _range(1e6f), _flags(RAYCAST_SOLID)			{}

};

#endif