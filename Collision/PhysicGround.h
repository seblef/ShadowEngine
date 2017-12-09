
#ifndef _PHYSICGROUND_H_
#define _PHYSICGROUND_H_

#include "PhysicObject.h"

class PhysicGround : public PhysicObject
{
public:

	PhysicGround(PxPhysics* physic, PxScene* scene, PhysicMaterial* m,
		int height, int width);
	~PhysicGround();
};

#endif