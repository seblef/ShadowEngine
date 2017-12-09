
#ifndef _GAMEENTITY_H_
#define _GAMEENTITY_H_

#include "GameObject.h"
#include "EntityTemplate.h"

class GameEntity : public GameObject
{
public:

	enum EntityType
	{
		TRIGGER=0,
		DYNAMIC,

		COUNT
	};

protected:

	EntityType				_entityType;
	bool					_needUpdate;

	EntityTemplate*			_template;

	MeshInstance*			_rMesh;
	IPhysicObject*			_physic;

public:

	GameEntity(EntityTemplate& t, const Matrix4& world, bool needUpdate, EntityType type);
	virtual ~GameEntity();

	EntityType				getEntityType() const				{ return _entityType; }

	void					onAddToScene();
	void					onRemFromScene();

	bool					needUpdate() const					{ return _needUpdate; }

	void					wakeUp();
	void					sleep();

	virtual void			update(float time)					{}
	virtual void			onUpdateMatrix(const Matrix4& world);
};

#endif