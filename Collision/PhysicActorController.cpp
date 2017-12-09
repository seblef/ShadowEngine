
#include "stdafx.h"
#include "PhysicActorController.h"
#include "Physic.h"
#include "PhysicUtils.h"

extern ofstream g_MediaOut;

PhysicActorController::PhysicActorController(const PhysicActorCtrlCreate_t& c,
											 PxPhysics* p, PxScene* s, PxControllerManager* m) :
	 PhysicObject(POBJ_ACTOR,s,c._material)
{
	_offset=c._box.getCenter();
	Vector3 size(c._box.getSize());
	size*=0.5f;

	if(c._type==PACTOR_AABBOX)
	{
		PxBoxControllerDesc d;
		d.setToDefault();
		d.halfForwardExtent=size.z;
		d.halfHeight=size.y;
		d.halfSideExtent=size.x;
		d.stepOffset=c._stepOffset;
		d.upDirection=PxVec3(0,1,0);
		d.material=c._material->getMaterial();
		d.callback = this;
		assert(d.isValid());

		_controller=m->createController(*p,s,d);

		_actorGeo = new PxBoxGeometry(PXVEC_C(size));
	}
	else
	{
		PxCapsuleControllerDesc d;
		d.setToDefault();
		d.height=size.y*2.0f;
		d.radius=smax(size.x,size.z);
		d.stepOffset=c._stepOffset;
		d.upDirection=PxVec3(0,1,0);
		d.material=c._material->getMaterial();
		d.callback = this;
		assert(d.isValid());

		_controller=m->createController(*p,s,d);

		_actorGeo = new PxCapsuleGeometry(d.radius, d.height);
	}
	
	_actor = _controller->getActor();
	_actor->userData = (IPhysicObject*)this;
	
	PxFilterData filterData;
	filterData.word0 = ACTORGROUP_ACTOR;
	filterData.word1 = ACTORGROUP_AMMO | ACTORGROUP_DYNAMIC | ACTORGROUP_TRIGGER;

	PxShape *shapes[16];
	_actor->getShapes(shapes, 16);
	for (int i = 0; i < _actor->getNbShapes(); ++i)
	{
		shapes[i]->setQueryFilterData(filterData);
		shapes[i]->setSimulationFilterData(filterData);
	}

	assert(_controller);
	setPosition(c._world);
}

PhysicActorController::~PhysicActorController()
{
	_controller->release();
	delete _actorGeo;
}

const Vector3& PhysicActorController::move(const Vector3& m, float elapsedTime)
{
	_controller->move(PxVec3(m.x,m.y,m.z),0.001f,elapsedTime,PxControllerFilters());
	PxExtendedVec3 p(_controller->getPosition());

	_position.x=p.x;
	_position.y=p.y;
	_position.z=p.z;

	return _position;
}

void PhysicActorController::addToScene()
{
	assert(0 && "Physic bad call: PhysicActorController::addToScene");
}

void PhysicActorController::remFromScene()
{
	assert(0 && "Physic bad call: PhysicActorController::remFromScene");
}

void PhysicActorController::onShapeHit(const PxControllerShapeHit& hit)
{
	IPhysicObject *o = (IPhysicObject*)hit.shape->getActor().userData;
	if (o)
	{
		PhysicObjectType t = o->getObjectType();
		if (t == POBJ_TRIGGER)
			Physic::getSingletonRef().postEvent(PhysicEvent(PhysicEvent::TRIGGER, this, o));
		else if (t == POBJ_AMMO)
			Physic::getSingletonRef().postEvent(PhysicEvent(PhysicEvent::CONTACT, o, this));
		else if (t==POBJ_DYNAMIC)
			Physic::getSingletonRef().postEvent(PhysicEvent(PhysicEvent::CONTACT, this, o));
	}
}

void PhysicActorController::onControllerHit(const PxControllersHit& hit)
{
}

void PhysicActorController::onObstacleHit(const PxControllerObstacleHit& hit)
{
}

float PhysicActorController::getDistanceFromPoint(const Vector3& p, Vector3& hitPoint)
{
	return PxGeometryQuery::pointDistance(PXVEC_C(p), *_actorGeo,
		_actor->getGlobalPose(), &(PXVEC(hitPoint)));
}