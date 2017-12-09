
#ifndef _PHYSICDYNAMIC_H_
#define _PHYSICDYNAMIC_H_

#include "PhysicObject.h"
#include "PhysicGeometry.h"

class PhysicDynamic : public PhysicObject
{
protected:

	Matrix4			_world;

public:

	PhysicDynamic(const PhysicGeometry& geo,
		const Matrix4& world, PhysicMaterial *m,
		PxPhysics* p, PxScene* s, float density);
	~PhysicDynamic();

	void			updateWorldMatrix();
	const Matrix4&	getWorldMatrix()			{ return _world; }

	void			addForce(const Vector3& force, bool impulse = false);
	void			addForce(const Vector3& force, const Vector3& pos, bool impulse = false);
	void			addTorque(const Vector3& torque, bool impulse = false);
};

#endif