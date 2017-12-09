
#ifndef _PHYSICDEFS_H_
#define _PHYSICDEFS_H_

#include <Core.h>

class PhysicMaterial;

using namespace Core;

#define ACTORGROUP_SCENE			(1 << 0)
#define ACTORGROUP_ACTOR			(1 << 1)
#define ACTORGROUP_TRIGGER			(1 << 2)
#define ACTORGROUP_AMMO				(1 << 3)
#define ACTORGROUP_DYNAMIC			(1 << 4)

enum PhysicShape
{
	PSHAPE_BOX,
	PSHAPE_SPHERE,
	PSHAPE_CAPSULE,
	PSHAPE_MESH,
	PSHAPE_CONVEXHULL,

	PSHAPE_COUNT
};

const char * const g_PhysicShapeNames[PSHAPE_COUNT]={
	"Box",
	"Sphere",
	"Capsule",
	"Mesh",
	"Convex Hull"
};

enum PhysicActorControllerType
{
	PACTOR_AABBOX,
	PACTOR_CAPSULE,

	PACTOR_COUNT
};

enum PhysicObjectType
{
	POBJ_ACTOR=0,
	POBJ_STATICSCENE,
	POBJ_KINEMATICSCENE,
	POBJ_DYNAMIC,
	POBJ_GROUND,
	POBJ_TRIGGER,
	POBJ_AMMO,

	POBJ_COUNT
};



struct PhysicGeometryCreate_t
{
	PhysicShape				_shape;
	BBox3					_box;
	int						_vxCount;
	VertexType				_vxType;
	const void*				_vx;
	int						_triCount;
	const unsigned short*	_tri;
};

struct PhysicActorCtrlCreate_t
{
	PhysicActorControllerType	_type;
	Matrix4						_world;
	BBox3						_box;
	float						_stepOffset;
	PhysicMaterial*				_material;
};

#define RAYCAST_SCENE				ACTORGROUP_SCENE
#define RAYCAST_ACTOR				ACTORGROUP_ACTOR
#define RAYCAST_TRIGGER				ACTORGROUP_TRIGGER
#define RAYCAST_DYNAMIC				ACTORGROUP_DYNAMIC
#define RAYCAST_AMMO				ACTORGROUP_AMMO

#define RAYCAST_ALL					(RAYCAST_SCENE | RAYCAST_ACTOR | RAYCAST_TRIGGER | RAYCAST_TRIGGER | RAYCAST_AMMO)
#define RAYCAST_SOLID				(RAYCAST_SCENE | RAYCAST_ACTOR | RAYCAST_DYNAMIC)


#endif