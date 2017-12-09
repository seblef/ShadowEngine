
#include "stdafx.h"
#include "PhysicGeometry.h"

PhysicGeometry::PhysicGeometry(const PhysicGeometryCreate_t& c, PxPhysics *p, PxCooking* cook) : _geo(0), _needPreTransform(false), _preTransform(Matrix4::Identity)
{
	switch(c._shape)
	{
	case PSHAPE_BOX:				makeBox(c,p);				break;
	case PSHAPE_SPHERE:				makeSphere(c,p);			break;
	case PSHAPE_CAPSULE:			makeCapsule(c,p);			break;
	case PSHAPE_MESH:				makeMesh(c,p,cook);			break;
	case PSHAPE_CONVEXHULL:			makeConvexHull(c,p,cook);	break;
	default:
		assert(0);
	}

}

PhysicGeometry::~PhysicGeometry()
{
	delete _geo;
}

void PhysicGeometry::makeBox(const PhysicGeometryCreate_t& c, PxPhysics *p)
{
	Vector3 size(c._box.getSize());
	size*=0.5f;

	_geo=new PxBoxGeometry(size.x,size.y,size.z);
	makeOffsetPreTransform(c._box.getCenter());
}

void PhysicGeometry::makeSphere(const PhysicGeometryCreate_t& c, PxPhysics *p)
{
	Vector3 size(c._box.getSize());
	float radius=smax(smax(size.x,size.y),size.z) * 0.5f;

	_geo=new PxSphereGeometry(radius);
	makeOffsetPreTransform(c._box.getCenter());
}

void PhysicGeometry::makeCapsule(const PhysicGeometryCreate_t& c, PxPhysics *p)
{
	Vector3 size(c._box.getSize());
	int axis;
	float r,h;

	if(size.y > size.x)
		axis=size.z > size.y ? 2 : 1;
	else
		axis=size.z > size.x ? 2 : 0;

	switch(axis)
	{
		case 0:
			r=size.y > size.z ? size.y : size.z;
			h=size.x;
			break;

		case 1:
			r=size.x > size.z ? size.x : size.z;
			h=size.y;
			_needPreTransform=true;
			_preTransform.createRotateZ(M_PI * 0.5f);
			break;

		case 2:
			r=size.x > size.y ? size.x : size.y;
			h=size.z;
			_needPreTransform=true;
			_preTransform.createRotateY(M_PI * 0.5f);
			break;
	}

	_preTransform=c._box.getCenter();
	_geo=new PxCapsuleGeometry(r*0.5f,h*0.5f);
}

void PhysicGeometry::makeMesh(const PhysicGeometryCreate_t& c, PxPhysics *p, PxCooking* cook)
{
	PxTriangleMeshDesc desc;
	PxDefaultMemoryOutputStream stream;

	desc.points.count=c._vxCount;
	desc.points.data=c._vx;
	desc.points.stride=g_VertexSize[c._vxType];
	desc.triangles.count=c._triCount;
	desc.triangles.data=(const void*)c._tri;
	desc.triangles.stride=3*sizeof(unsigned short);
	desc.flags=PxMeshFlag::e16_BIT_INDICES;

	cook->cookTriangleMesh(desc,stream);

	PxDefaultMemoryInputData input(stream.getData(),stream.getSize());
	PxTriangleMesh *mesh=p->createTriangleMesh(input);
	_geo=new PxTriangleMeshGeometry(mesh);

//	mesh->release();
}

void PhysicGeometry::makeConvexHull(const PhysicGeometryCreate_t& c, PxPhysics *p, PxCooking* cook)
{
	PxConvexMeshDesc desc;
	PxDefaultMemoryOutputStream stream;

	desc.points.count=c._vxCount;
	desc.points.data=c._vx;
	desc.points.stride=g_VertexSize[c._vxType];
	desc.flags=PxConvexFlag::eCOMPUTE_CONVEX;

	cook->cookConvexMesh(desc,stream);

	PxDefaultMemoryInputData input(stream.getData(),stream.getSize());
	PxConvexMesh *convex=p->createConvexMesh(input);

	_geo=new PxConvexMeshGeometry(convex);
//	convex->release();
}
