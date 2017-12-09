
#include "stdafx.h"
#include "PhysicObject.h"
#include "PhysicUtils.h"


PhysicObject::PhysicObject(PhysicObjectType type, PxScene* scene, PhysicMaterial* mat,
	const PhysicGeometry* geo) :
	IPhysicObject(type), _scene(scene), _material(mat), _actor(0), _geometry(geo)
{

}

PhysicObject::~PhysicObject()
{

}

void PhysicObject::addToScene()
{
	_scene->addActor(*_actor);
}

void PhysicObject::remFromScene()
{
	_scene->removeActor(*_actor);
}

void PhysicObject::setWorldMatrix(const Matrix4& world)
{
	assert(0 && "Physic bad call: setWorldMatrix");
}

const Matrix4& PhysicObject::getWorldMatrix()
{
	assert(0 && "Physic bad call: getWorldMatrix");
	return Matrix4::Identity;
}

PxShape* PhysicObject::makeShape(const PhysicGeometry& geo,
	PhysicMaterial* mat, unsigned int actorGroup, unsigned int collideWith) const
{
	PxShape *shape;
	if (geo.needPreTransform())
	{
		PMAKETRANSFORM(pre, geo.getPreTransform());
		shape = _actor->createShape(geo.getGeometry(), *mat->getMaterial(), pre);
	}
	else
		shape = _actor->createShape(geo.getGeometry(), *mat->getMaterial());

	PxFilterData filterData;
	filterData.word0 = actorGroup;
	filterData.word1 = collideWith;

	shape->setQueryFilterData(filterData);
	shape->setSimulationFilterData(filterData);
	return shape;
}

float PhysicObject::getDistanceFromPoint(const Vector3& p, Vector3& hitPoint)
{
	if (!_geometry || _geometry->getGeometry().getType() == PxGeometryType::eHEIGHTFIELD ||
		_geometry->getGeometry().getType() == PxGeometryType::eTRIANGLEMESH)
		return -1.0f;

	return PxGeometryQuery::pointDistance(PXVEC_C(p), _geometry->getGeometry(),
		_actor->getGlobalPose(),&(PXVEC(hitPoint)));
}