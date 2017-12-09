
#ifndef _PHYSICACTORCONTROLLER_H_
#define _PHYSICACTORCONTROLLER_H_

#include "PhysicObject.h"


class PhysicActorController : public PhysicObject, public PxUserControllerHitReport
{
protected:

	PxController*				_controller;
	Vector3						_offset;
	Vector3						_position;

	PxGeometry*					_actorGeo;

public:

	PhysicActorController(const PhysicActorCtrlCreate_t& c,
		PxPhysics* p, PxScene* s, PxControllerManager* m);
	~PhysicActorController();


	const Vector3&				move(const Vector3& m, float elpasedTime);
	
	void						setPosition(const Vector3& p)
	{
		_position=p;
		_controller->setPosition(PxExtendedVec3(p.x + _offset.x,p.y + _offset.y,p.z + _offset.z));
	}
	const Vector3&				getPosition() const				{ return _position; }

	void						addToScene();
	void						remFromScene();

	void						onShapeHit(const PxControllerShapeHit& hit);
	void						onControllerHit(const PxControllersHit& hit);
	void						onObstacleHit(const PxControllerObstacleHit& hit);

	float						getDistanceFromPoint(const Vector3& p, Vector3& hitPoint);
};

#endif