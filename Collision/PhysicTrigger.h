
#ifndef _PHYSICTRIGGER_H_
#define _PHYSICTRIGGER_H_

#include "PhysicObject.h"

class PhysicTrigger : public PhysicObject
{
protected:

	Matrix4				_world;

public:

	PhysicTrigger(const PhysicGeometry& geo,
		const Matrix4& world, PhysicMaterial *m,
		PxPhysics* p, PxScene* s);
	~PhysicTrigger();

	void				setWorldMatrix(const Matrix4& world);
	const Matrix4&		getWorldMatrix()			{ return _world; }
};

#endif