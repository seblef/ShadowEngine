
#ifndef _PHYSICAMMO_H_
#define _PHYSICAMMO_H_

#include "PhysicObject.h"

class PhysicAmmo : public PhysicObject
{
protected:

	Matrix4				_world;

public:

	PhysicAmmo(const PhysicGeometry& geo,
		const Matrix4& world, PhysicMaterial *m,
		PxPhysics* p, PxScene* s);
	~PhysicAmmo();

	void				setWorldMatrix(const Matrix4& world);
	const Matrix4&		getWorldMatrix()							{ return _world; }
};


#endif