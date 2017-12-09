
#ifndef _FIELDHIT_H_
#define _FIELDHIT_H_

#include <Core.h>

using namespace Core;

class FieldHit
{
public:

	FieldHit()			{}

	static void			hitField(const Vector3& center, float innerRadius,
		float outerRadius, float innerDamage);
};

#endif