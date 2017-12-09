
#ifndef _PHYSICMATERIAL_H_
#define _PHYSICMATERIAL_H_

#include <PxPhysicsAPI.h>

using namespace physx;

class PhysicMaterial
{
protected:

	PxMaterial*			_material;

public:

	PhysicMaterial(float staticFriction, float dynamicFriction, float restitution,
		PxPhysics* physic)
	{
		_material = physic->createMaterial(staticFriction, dynamicFriction, restitution);
	}
	~PhysicMaterial()
	{
		_material->release();
	}

	PxMaterial*			getMaterial() const				{ return _material; }
};

#endif