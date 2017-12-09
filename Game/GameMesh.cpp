
#include "stdafx.h"
#include "GameMesh.h"
#include "GameSystem.h"
#include "BBoxToRect.h"
#include <StdRenderer.h>


GameMesh::GameMesh(const TemplateMesh& t, const Matrix4& world, bool alwaysVisible) : GameObject(MESH,world,t.getGeometry().getBBox(),t.getFlags()),
	_physic(0), _template(&t)
{
	_rInstance=new MeshInstance(&t.getMesh(),world,alwaysVisible);
	if(t.getPhysicGeometry())
	{
		_physic=Physic::getSingletonRef().createStaticScene(*t.getPhysicGeometry(),world);
		_physic->setUserData((GameObject*)this);
	}
}

GameMesh::~GameMesh()
{
	delete _rInstance;
	if(_physic)
		delete _physic;
}

void GameMesh::onAddToScene()
{
	Renderer::getSingletonRef().addRenderable(_rInstance);
	if (_physic)
		_physic->addToScene();
}

void GameMesh::onRemFromScene()
{
	Renderer::getSingletonRef().remRenderable(_rInstance);
	if (_physic)
		_physic->remFromScene();
}