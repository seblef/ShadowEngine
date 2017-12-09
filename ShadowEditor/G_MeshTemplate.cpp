
#include "stdafx.h"
#include "G_MeshTemplate.h"
#include "G_Selection.h"
#include "../Game/GeometryLoader.h"

G_MeshTemplate::G_MeshTemplate(const string& name, const string& geoFile,
							   const string& matName, PhysicShape ps,
							   unsigned int flags) : _name(name), _geoFile(geoFile), _matName(matName),
							   _shape(ps), _flags(flags), _gameMaterial(0)
{
	_geo=GeometryLoader::loadGeometry(geoFile);
	_mat=MaterialSystem::getSingletonRef().getMaterial(matName);

	_rMesh=new Mesh(_geo->getVertexCount(),_geo->getVertices(),_geo->getTriangleCount(),_geo->getTriangles(),_mat,Renderer::getSingletonRef().getVideoDevice());
	_sMesh=G_Selection::getSingletonRef().createMeshGeometry(*_geo);
}

G_MeshTemplate::~G_MeshTemplate()
{
	delete _rMesh;
	delete _geo;

	G_Selection::getSingletonRef().deleteMeshGeometry(_sMesh);
}

void G_MeshTemplate::setMaterial(const string& matName)
{
	_matName=matName;
	_mat=MaterialSystem::getSingletonRef().getMaterial(matName);
	_rMesh->setMaterial(_mat);
}
