
#ifndef _IPHYSICOBJECT_H_
#define _IPHYSICOBJECT_H_

#include "PhysicDefs.h"

class IPhysicObject
{
private:

	PhysicObjectType		_type;
	void*					_user;

	unsigned int			_queryID;

public:

	IPhysicObject(PhysicObjectType t) : _type(t), _user(0), _queryID(0)			{}
	virtual ~IPhysicObject()						{}

	PhysicObjectType		getObjectType() const				{ return _type; }

	void					setUserData(void *d)				{ _user = d; }
	void*					getUserData() const					{ return _user; }

	void					setQueryID(unsigned int id)			{ _queryID = id; }
	unsigned int			getQueryID() const					{ return _queryID; }

	virtual void			addToScene() = 0;
	virtual void			remFromScene() = 0;

	virtual void			setWorldMatrix(const Matrix4& world) = 0;
	virtual const Matrix4&	getWorldMatrix() = 0;

	virtual float			getDistanceFromPoint(const Vector3& p, Vector3& hitPoint) = 0;
};

#endif