
#ifndef _HIT_H_
#define _HIT_H_

#include <Core.h>

using namespace Core;

class GameObject;

class Hit
{
public:

	GameObject*			_object;
	GameObject*			_from;
	Vector3				_impact;
	Vector3				_dir;
	float				_damage;

	Hit()				{}
	Hit(GameObject* o, GameObject* from, const Vector3& impact, const Vector3& dir, float damage) :
		_object(o), _impact(impact), _dir(dir), _damage(damage), _from(from)			{}
	Hit(const Hit& h) : _object(h._object), _impact(h._impact), _dir(h._dir),
		_from(h._from), _damage(h._damage)		{}

};
#endif