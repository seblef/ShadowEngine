
#include "stdafx.h"
#include "FieldHit.h"
#include "GameCharacter.h"
#include "GameEntityDynamic.h"
#include <PhysicLib.h>

extern ofstream g_MediaOut;

void FieldHit::hitField(const Vector3& center, float innerRadius,
	float outerRadius, float innerDamage)
{
	PhysicQuery q;
	q._center = center;
	q._shape = PSHAPE_SPHERE;
	q._groupMask = RAYCAST_ACTOR | RAYCAST_DYNAMIC;
	q._size.x = outerRadius;

	Physic::getSingletonRef().query(q);

	if (q._objectCount == 0)
		return;

	if (outerRadius <= innerRadius)
		outerRadius = innerRadius + 0.01f;

	float dgradient = 1.0f / (outerRadius - innerRadius);
	float dist;
	float fact;
	IPhysicObject* o;
	Vector3 impact;
	Hit h;
	h._from = 0;

	for (unsigned int i = 0; i < q._objectCount; ++i)
	{
		o = q._objects[i];
		dist = o->getDistanceFromPoint(center, impact);

		if (dist < innerRadius)
			fact = 1.0f;
		else
			fact = 1.0f - (dist - innerRadius) * dgradient;

		h._dir = impact - center;
		h._dir.normalize();
		h._impact = impact;
		h._damage = smax(innerDamage * fact,0.0f);
		h._object = (GameObject*)o->getUserData();

		if (o->getObjectType() == POBJ_ACTOR)
			((GameCharacter*)h._object)->onHit(h);
		else if (o->getObjectType() == POBJ_DYNAMIC)
			((GameEntityDynamic*)h._object)->onHit(h);
	}
}