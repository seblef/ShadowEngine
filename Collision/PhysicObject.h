
#ifndef _PHYSICOBJECT_H_
#define _PHYSICOBJECT_H_

#include "IPhysicObject.h"
#include "PhysicMaterial.h"
#include "PhysicGeometry.h"

using namespace physx;

class PhysicObject : public IPhysicObject
{
protected:

	PxRigidActor*			_actor;
	PxScene*				_scene;
	PhysicMaterial*			_material;
	const PhysicGeometry*	_geometry;

	PxShape*				makeShape(const PhysicGeometry& geo, 
		PhysicMaterial *mat, unsigned int actorGroup, unsigned int collideWith) const;

public:

	PhysicObject(PhysicObjectType type, PxScene* scene, PhysicMaterial* mat,
		const PhysicGeometry* geo = 0);
	virtual ~PhysicObject();

	virtual void			addToScene();
	virtual void			remFromScene();

	virtual void			setWorldMatrix(const Matrix4& world);
	virtual const Matrix4&	getWorldMatrix();

	virtual float			getDistanceFromPoint(const Vector3& p, Vector3& hitPoint);
};

#endif