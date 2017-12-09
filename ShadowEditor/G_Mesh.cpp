
#include "stdafx.h"
#include "G_Mesh.h"
#include "G_Selection.h"

G_Mesh::G_Mesh(G_MeshTemplate* t) : G_Object(MESH), _template(t)
{
	_selItem=G_Selection::getSingletonRef().createMeshActor(t->getSelectionMesh());
	_selItem->setUserData((G_Object*)this);

	_localBox=t->getGeometry()->getBBox();
}

G_Mesh::G_Mesh(const G_Mesh& m) : G_Object(&m), _template(m._template)
{
	_selItem=G_Selection::getSingletonRef().createMeshActor(_template->getSelectionMesh());
	_selItem->setUserData((G_Object*)this);
}

void G_Mesh::rebuildMatrix()
{
	G_Object::rebuildMatrix();

	if(_rMesh)		_rMesh->setWorldMatrix(_world);
}

void G_Mesh::onAddToScene()
{
	_rMesh=new MeshInstance(_template->getMesh(),_world,false);

	Renderer::getSingletonRef().addRenderable(_rMesh);
	G_Object::onAddToScene();
}

void G_Mesh::onRemFromScene()
{
	Renderer::getSingletonRef().remRenderable(_rMesh);
	delete _rMesh;
	_rMesh=0;

	G_Object::onRemFromScene();
}