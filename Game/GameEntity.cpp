
#include "stdafx.h"
#include "GameEntity.h"
#include "EntityUpdater.h"

GameEntity::GameEntity(EntityTemplate& t, const Matrix4& world, bool needUpdate, EntityType type) : GameObject(ENTITY,world,BBox3::NullBox,0),
	_needUpdate(needUpdate), _entityType(type), _template(&t), _rMesh(0), _physic(0)
{
	t.load();
	if (t.getMesh())
		_rMesh = new MeshInstance(t.getMesh(), world, false);
}

GameEntity::~GameEntity()
{
	if (_rMesh)			delete _rMesh;
	if (_physic)		delete _physic;
}

void GameEntity::onAddToScene()
{
	if (_rMesh)			Renderer::getSingletonRef().addRenderable(_rMesh);
	if (_physic)		_physic->addToScene();

	if (_template->isTrigger() && _needUpdate)
		wakeUp();
}

void GameEntity::onRemFromScene()
{
	EntityUpdater::getSingletonRef().remEntity(this);

	if (_rMesh)			Renderer::getSingletonRef().remRenderable(_rMesh);
	if (_physic)		_physic->remFromScene();
}

void GameEntity::wakeUp()
{
	EntityUpdater::getSingletonRef().addEntity(this);
}

void GameEntity::sleep()
{
	EntityUpdater::getSingletonRef().remEntity(this);
}

void GameEntity::onUpdateMatrix(const Matrix4& world)
{
	if (_rMesh)			_rMesh->setWorldMatrix(world);
}
