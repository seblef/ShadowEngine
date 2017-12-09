
#include "stdafx.h"
#include "Physic.h"
#include "PhysicGround.h"
#include "PhysicStaticScene.h"
#include "PhysicKinematicScene.h"
#include "PhysicActorController.h"
#include "PhysicTrigger.h"
#include "PhysicAmmo.h"
#include "PhysicUtils.h"

extern ofstream g_MediaOut;


static PxDefaultErrorCallback			g_DefaultErrorCallback;
static PxDefaultAllocator				g_DefaultAllocatorCallback;


PxFilterFlags PhysicFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if ((PxFilterObjectIsTrigger(attributes0) && (filterData1.word0 & ACTORGROUP_ACTOR)) || 
		(PxFilterObjectIsTrigger(attributes1) && (filterData0.word0 & ACTORGROUP_ACTOR)))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
 
	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}


Physic::Physic(float updateTime, const Vector3& gravity, bool debugMode) : _updateTime(updateTime),
	_time(0), _connection(0), _zoneManager(0), _events(128), _queryID(0)
{
	_foundation=PxCreateFoundation(PX_PHYSICS_VERSION, g_DefaultAllocatorCallback, g_DefaultErrorCallback);
	assert(_foundation);

	if(debugMode)		_zoneManager=&PxProfileZoneManager::createProfileZoneManager(_foundation);

	_physic=PxCreatePhysics(PX_PHYSICS_VERSION,*_foundation,PxTolerancesScale(),debugMode,_zoneManager);
	assert(_physic);

	_cooking=PxCreateCooking(PX_PHYSICS_VERSION,*_foundation,PxCookingParams());
	assert(_cooking);

	PxInitExtensions(*_physic);

	PxSceneDesc desc(_physic->getTolerancesScale());
	desc.cpuDispatcher=PxDefaultCpuDispatcherCreate(1);
	desc.filterShader = PhysicFilterShader;
	desc.gravity.x=gravity.x;
	desc.gravity.y=gravity.y;
	desc.gravity.z=gravity.z;
	desc.flags|=PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS | PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
	desc.simulationEventCallback=this;
	_scene=_physic->createScene(desc);
	_scene->setSimulationEventCallback(this);
	assert(_scene);

	_ctrlMgr=PxCreateControllerManager(*_foundation);
	assert(_ctrlMgr);

	if(debugMode && _physic->getPvdConnectionManager())
	{
		_connection=PxVisualDebuggerExt::createConnection(_physic->getPvdConnectionManager(),
			"localhost",5425,1000,PxVisualDebuggerExt::getAllConnectionFlags());
	}

	_defaultMat = new PhysicMaterial(0.5f, 0.5f, 0.5f, _physic);
}

Physic::~Physic()
{
	delete _defaultMat;

	if(_connection)			_connection->release();

	_scene->release();
	_ctrlMgr->release();
	_cooking->release();
	_physic->release();

	PxCloseExtensions();

	if(_zoneManager)		_zoneManager->release();

	_foundation->release();
}

void Physic::update(float time)
{
	_time+=time;
//	_events.clear();

	if(_time >= _updateTime)
	{
		while(_time >= _updateTime)
		{
			_scene->simulate(_updateTime);
			_time-=_updateTime;
		}

		_scene->fetchResults(true);
	}

	DynamicSet::iterator d(_activeDynamics.begin());
	for (; d != _activeDynamics.end(); ++d)
		(*d)->updateWorldMatrix();
}

const PhysicEvent* Physic::getEvents(int& count) const
{
	count = _events.getCount();
	return _events.getBuffer();
}

bool Physic::testRay(const RayCastInfos& rc) const
{
	PxRaycastHit hit;
	PxSceneQueryFilterData filterData;
	filterData.data.word0 = rc._flags;

	return _scene->raycastAny(PXVEC_C(rc._origin), PXVEC_C(rc._dir) ,
		rc._range, hit, filterData);
}

void Physic::query(PhysicQuery& q)
{
	++_queryID;

	PxTransform pose;
	pose.createIdentity();
	pose.p = PXVEC_C(q._center);
	pose.q.normalize();
	assert(pose.isValid());

	PxSceneQueryFilterData filter;
	filter.data.word0 = q._groupMask;

	PxShape* hitBuffer[PHYSICQUERY_MAXOBJECTS];
	int res=0;

	switch (q._shape)
	{
	case PSHAPE_BOX:
		res = _scene->overlapMultiple(PxBoxGeometry(PXVEC_C(q._size)), pose, hitBuffer,
			PHYSICQUERY_MAXOBJECTS, filter);
		break;

	case PSHAPE_SPHERE:
		res = _scene->overlapMultiple(PxSphereGeometry(q._size.x), pose, hitBuffer,
			PHYSICQUERY_MAXOBJECTS, filter);
		break;

	case PSHAPE_CAPSULE:
		res = _scene->overlapMultiple(PxCapsuleGeometry(q._size.x, q._size.y), pose, hitBuffer,
			PHYSICQUERY_MAXOBJECTS, filter);
		break;
	}

	if (res == -1)
	{
#ifdef _DEBUG
		g_MediaOut << "x [Physic::query]: Overflow in hit buffer." << endl;
#endif
		res = 0;
	}

	q._objectCount = 0;
	IPhysicObject* o;

	for (int i = 0; i < res; ++i)
	{
		o = (IPhysicObject*)hitBuffer[i]->getActor().userData;
		if (o && o->getQueryID() != _queryID)
		{
			q._objects[q._objectCount] = o;
			++q._objectCount;
			o->setQueryID(_queryID);
		}
	}
}

IPhysicObject* Physic::rayCast(RayCastInfos& rc) const
{
	PxRaycastHit hit;
	PxSceneQueryFilterData filterData;
	filterData.data.word0 = rc._flags;

	if (_scene->raycastSingle(PXVEC_C(rc._origin), PXVEC_C(rc._dir),
		rc._range, PxSceneQueryFlag::eIMPACT, hit, filterData))
	{
		return (IPhysicObject*)hit.shape->getActor().userData;
	}
	else
		return 0;
}

IPhysicObject* Physic::rayCastImpact(RayCastInfos& rc) const
{
	PxRaycastHit hit;
	PxSceneQueryFilterData filterData;
	filterData.data.word0 = rc._flags;

	if (_scene->raycastSingle(PXVEC_C(rc._origin), PXVEC_C(rc._dir), rc._range,
		PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL, hit, filterData))
	{
		rc._impact.x = hit.impact.x;
		rc._impact.y = hit.impact.y;
		rc._impact.z = hit.impact.z;
		rc._normal.x = hit.normal.x;
		rc._normal.y = hit.normal.y;
		rc._normal.z = hit.normal.z;
		return (IPhysicObject*)hit.shape->getActor().userData;
	}
	else
		return 0;
}

IPhysicObject* Physic::createGround(int w, int h, PhysicMaterial *m) const
{
	return new PhysicGround(_physic, _scene, m ? m : _defaultMat, w, h);
}

IPhysicObject* Physic::createStaticScene(const PhysicGeometry& geo, const Matrix4& world,
	PhysicMaterial* m) const
{
	return new PhysicStaticScene(geo, world, m ? m : _defaultMat, _physic, _scene);
}

IPhysicObject* Physic::createKinematicScene(const PhysicGeometry& geo, const Matrix4& world,
	PhysicMaterial* m) const
{
	return new PhysicKinematicScene(geo, world, m ? m : _defaultMat, _physic, _scene);
}

IPhysicObject* Physic::createActorController(PhysicActorCtrlCreate_t& c) const
{
	if (c._material == 0)
		c._material = _defaultMat;

	return new PhysicActorController(c, _physic, _scene, _ctrlMgr);
}

IPhysicObject* Physic::createDynamic(const PhysicGeometry& geo, const Matrix4& world,
	float density, PhysicMaterial* m) const
{
	return new PhysicDynamic(geo, world, m ? m : _defaultMat, _physic, _scene, density);
}

IPhysicObject* Physic::createTrigger(const PhysicGeometry& geo, const Matrix4& world) const
{
	return new PhysicTrigger(geo, world, _defaultMat, _physic, _scene);
}

IPhysicObject* Physic::createAmmo(const PhysicGeometry& geo, const Matrix4& world) const
{
	return new PhysicAmmo(geo, world, _defaultMat, _physic, _scene);
}

void Physic::onWake(PxActor** actors, PxU32 count)
{
	IPhysicObject* o;
	for (PxU32 i = 0; i < count; ++i)
	{
		o = (IPhysicObject*)actors[i]->userData;
		if (o && o->getObjectType() == POBJ_DYNAMIC)
		{
			_activeDynamics.insert((PhysicDynamic*)o);
			_events.add(PhysicEvent(PhysicEvent::WAKEUP, o, 0));
		}
	}
}

void Physic::onSleep(PxActor** actors, PxU32 count)
{
	IPhysicObject* o;
	for (PxU32 i = 0; i < count; ++i)
	{
		o = (IPhysicObject*)actors[i]->userData;
		if (o && o->getObjectType() == POBJ_DYNAMIC)
		{
			_activeDynamics.erase((PhysicDynamic*)o);
			_events.add(PhysicEvent(PhysicEvent::SLEEP, o, 0));
		}
	}
}

void Physic::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	IPhysicObject* a1, *a2, *actor,*target;
	PhysicObjectType t1, t2;

	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		const PxContactPair& cp = pairs[i];
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			a1 = (IPhysicObject*)cp.shapes[0]->getActor().userData;
			a2 = (IPhysicObject*)cp.shapes[1]->getActor().userData;

			t1 = a1->getObjectType();
			t2 = a2->getObjectType();

			if (t1 == POBJ_AMMO)
			{
				actor = a1;
				target = a2;
			}
			else if (t2 == POBJ_AMMO)
			{
				actor = a2;
				target = a1;
			}
			else
				continue;

			_events.add(PhysicEvent(PhysicEvent::CONTACT, actor, target));
		}
	}
}

void Physic::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	g_MediaOut << "Physic::onTrigger " << count <<  endl;
	for (PxU32 i = 0; i < count; ++i)
	{
		if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			IPhysicObject *t = 0;
			IPhysicObject* a = 0;

			t = (IPhysicObject*)pairs[i].triggerShape->getActor().userData;
			a = (IPhysicObject*)pairs[i].otherShape->getActor().userData;

			assert(a && t && a->getObjectType() == POBJ_ACTOR && t->getObjectType() == POBJ_TRIGGER);
			_events.add(PhysicEvent(PhysicEvent::TRIGGER, a, t));
		}
	}
}

void Physic::remDynamicFromActiveList(PhysicDynamic* d)
{
	_activeDynamics.erase(d);
}
