
#ifndef _PHYSICQUERY_H_
#define _PHYSICQUERY_H_

#include "IPhysicObject.h"

#define PHYSICQUERY_MAXOBJECTS				32

class PhysicQuery
{
public:

	unsigned int			_groupMask;
	PhysicShape				_shape;
	Vector3					_center;
	Vector3					_size;

	unsigned int			_objectCount;
	IPhysicObject*			_objects[PHYSICQUERY_MAXOBJECTS];

	PhysicQuery() : _objectCount(0)			{};
	PhysicQuery(unsigned int groupMask, PhysicShape shape,
		const Vector3& center, const Vector3& size) : _groupMask(groupMask),
		_shape(shape), _center(center), _size(size), _objectCount(0)			{}
};

#endif