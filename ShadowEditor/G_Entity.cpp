
#include "stdafx.h"
#include "G_Entity.h"
#include "G_Selection.h"
#include "G_EditorRenderer.h"

G_Entity::G_Entity(G_EntityTemplate* t) : G_Object(ENTITY),
	_rMesh(0), _template(t)
{
	t->load();
	if (t->getMesh())
	{
		_selItem = G_Selection::getSingletonRef().createMeshActor(t->getPhysicMesh());
		_selItem->setUserData((G_Object*)this);
		_localBox = t->getGeometry()->getBBox();
	}
	else
	{
		_selItem = G_Selection::getSingletonRef().createSphereActor(0.25f);
		_selItem->setUserData((G_Object*)this);
	}
}

G_Entity::G_Entity(const G_Entity& e) : G_Object(&e), _template(e._template)
{
	if (_template->getMesh())
	{
		_selItem = G_Selection::getSingletonRef().createMeshActor(_template->getPhysicMesh());
		_selItem->setUserData((G_Object*)this);
		_localBox = _template->getGeometry()->getBBox();
	}
	else
	{
		_selItem = G_Selection::getSingletonRef().createSphereActor(0.25f);
		_selItem->setUserData((G_Object*)this);
	}
}

G_Entity::~G_Entity()
{
	if (_rMesh)
		delete _rMesh;
}

void G_Entity::onAddToScene()
{
	G_Object::onAddToScene();

	if (_template->getMesh())
	{
		_rMesh = new MeshInstance(_template->getMesh(), _world, false);
		Renderer::getSingletonRef().addRenderable(_rMesh);
	}
	else
		G_EditorRenderer::getSingletonRef().addObject(this);
}

void G_Entity::onRemFromScene()
{
	G_Object::onRemFromScene();

	if (_template->getMesh())
	{
		Renderer::getSingletonRef().remRenderable(_rMesh);
		delete _rMesh;
		_rMesh = 0;
	}
	else
		G_EditorRenderer::getSingletonRef().remObject(this);
}

void G_Entity::rebuildMatrix()
{
	G_Object::rebuildMatrix();

	if (_rMesh)		_rMesh->setWorldMatrix(_world);
}

void G_Entity::drawSelection(G_EditorObjDrawer* d)
{

}