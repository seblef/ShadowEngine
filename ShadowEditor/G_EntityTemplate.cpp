
#include "stdafx.h"
#include "G_EntityTemplate.h"
#include "G_Selection.h"
#include "../Game/GeometryLoader.h"

G_EntityTemplate::G_EntityTemplate(ScriptFile& sf) : _rMesh(0), _rMaterial(0), _physic(0), _geo(0)
{
	sf.getToken();		// Entity classname
	_name = sf.getToken();
	string t(sf.getToken());

	while (sf.good() && t != "end_entity")
	{
		if (t == "geometry")		_meshFile = sf.getToken();
		else if (t == "material")	_materialFile = sf.getToken();

		t = sf.getToken();
	}
}

G_EntityTemplate::~G_EntityTemplate()
{
	if (_geo)				delete _geo;
	if (_rMaterial)			delete _rMaterial;
	if (_rMesh)				delete _rMesh;

	if (_physic)			G_Selection::getSingletonRef().deleteMeshGeometry(_physic);
}

void G_EntityTemplate::load()
{
	if (!_rMesh && !_meshFile.empty() && !_materialFile.empty())
	{
		_geo = GeometryLoader::loadGeometry(_meshFile);
		assert(_geo);
		_rMaterial = MaterialSystem::getSingletonRef().loadMaterial(_materialFile);
		assert(_rMaterial);

		_rMesh = new Mesh(_geo->getVertexCount(), _geo->getVertices(),
			_geo->getTriangleCount(), _geo->getTriangles(),
			_rMaterial, Renderer::getSingletonRef().getVideoDevice());

		_physic = G_Selection::getSingletonRef().createMeshGeometry(*_geo);
	}
}
