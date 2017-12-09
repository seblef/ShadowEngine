
#ifndef _PHYSICSTATICSCENE_H_
#define _PHYSICSTATICSCENE_H_

#include "PhysicObject.h"
#include "PhysicGeometry.h"

class PhysicStaticScene : public PhysicObject
{
public:

	PhysicStaticScene(const PhysicGeometry& geo,
		const Matrix4& world, PhysicMaterial *m,
		PxPhysics* p, PxScene* s);
	~PhysicStaticScene();
};

#endif