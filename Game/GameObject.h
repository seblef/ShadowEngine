
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <Core.h>
#include "ObjectFlags.h"

using namespace Core;

class GameObject
{
public:
	enum ObjectType
	{
		MESH=0,
		LIGHT,
		SOUND,
		ACTOR,
		PARTICLES,
		ENTITY,

		COUNT
	};

protected:

	ObjectType				_type;
	unsigned int			_flags;
	Matrix4					_world;
	BBox3					_localBox;
	BBox3					_worldBox;

public:

	GameObject(ObjectType type, const Matrix4& world, const BBox3& localbox, unsigned int flags) :
		_type(type), _flags(flags), _world(world), _localBox(localbox)
	{
		_worldBox.transformFromBox(localbox,world);
	}
	virtual ~GameObject()																										{}

	ObjectType				getType() const									{ return _type; }

	unsigned int			getFlag(unsigned int f) const					{ return _flags & f; }
	const Matrix4&			getWorldMatrix() const							{ return _world; }
	const BBox3&			getLocalBBox() const							{ return _localBox; }
	const BBox3&			getWorldBBox() const							{ return _worldBox; }

	void					updateMatrix(const Matrix4& world)
	{
		onUpdateMatrix(world);
		_world=world;
		_worldBox.transformFromBox(_localBox,world);
	}

	virtual void			onAddToScene()									{}
	virtual void			onRemFromScene()								{}
	virtual void			onUpdateMatrix(const Matrix4& new_m)			{}
};

#endif