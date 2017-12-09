
#include "stdafx.h"
#include "EntityTemplate.h"
#include "GeometryLoader.h"

const char* const g_PhysicShapes[PSHAPE_COUNT] =
{
	"box",
	"sphere",
	"capsule",
	"mesh",
	"chull"
};

EntityTemplate::EntityTemplate(bool isTrigger) : _rMaterial(0), _rMesh(0), _pGeometry(0), _geometry(0), _pShape(PSHAPE_SPHERE),
	_isTrigger(isTrigger)
{
}

void EntityTemplate::load()
{
	if (!_rMesh && !_rMaterial && !_meshName.empty() && !_materialName.empty())
	{
		_geometry = GeometryLoader::loadGeometry(_meshName);
		assert(_geometry);
		_rMaterial = MaterialSystem::getSingletonRef().loadMaterial(_materialName);
		assert(_rMaterial);

		_rMesh = new Mesh(_geometry->getVertexCount(), _geometry->getVertices(),
			_geometry->getTriangleCount(), _geometry->getTriangles(),
			_rMaterial, Renderer::getSingletonRef().getVideoDevice());

		PhysicGeometryCreate_t c;
		c._box = _geometry->getBBox();
		c._shape = _pShape;
		c._tri = _geometry->getTriangles();
		c._triCount = _geometry->getTriangleCount();
		c._vx = _geometry->getVertices();
		c._vxCount = _geometry->getVertexCount();
		c._vxType = VX_3D;

		_pGeometry = Physic::getSingletonRef().createGeometry(c);
	}
}

void EntityTemplate::unload()
{
	if (_rMaterial)				delete _rMaterial;
	if (_rMesh)					delete _rMesh;
	if (_geometry)				delete _geometry;
	if (_pGeometry)				delete _pGeometry;

	_rMaterial = 0;
	_rMesh = 0;
	_geometry = 0;
	_pGeometry = 0;
}

void EntityTemplate::parseToken(const string& t, ScriptFile& sf)
{
	if (t == "geometry")			_meshName = sf.getToken();
	else if (t == "material")		_materialName = sf.getToken();
	else if (t == "shape")
	{
		const string& s = sf.getToken();
		for (int i = 0; i < PSHAPE_COUNT; ++i)
		{
			if (s == g_PhysicShapes[i])
				_pShape = (PhysicShape)i;
		}
	}
}
