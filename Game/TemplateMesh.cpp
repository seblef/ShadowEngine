
#include "stdafx.h"
#include "TemplateMesh.h"
#include "ObjectFlags.h"
#include "GeometryLoader.h"

TemplateMesh::TemplateMesh(Geometry* geo, Material* mat, GameMaterial *gmat, unsigned int flags,
	PhysicShape shape) : _geometry(geo), _material(mat), _flags(flags), _physic(0), _gameMaterial(gmat)
{
	_rMesh=new Mesh(geo->getVertexCount(),geo->getVertices(),geo->getTriangleCount(),geo->getTriangles(),mat,Renderer::getSingletonRef().getVideoDevice());

	if((flags & OF_NOCOLLISION)==0)
	{
		PhysicGeometryCreate_t c;
		c._box=geo->getBBox();
		c._shape=shape;
		c._tri=geo->getTriangles();
		c._triCount=geo->getTriangleCount();
		c._vx=geo->getVertices();
		c._vxCount=geo->getVertexCount();
		c._vxType=VX_3D;

		_physic=Physic::getSingletonRef().createGeometry(c);
	}
}

TemplateMesh::~TemplateMesh()
{
	delete _geometry;
	delete _rMesh;

	if(_physic)			delete _physic;
}

TemplateMesh* TemplateMesh::buildMesh(const string& geoFile, const string& matFile,
	GameMaterial *gMat, unsigned int flags, PhysicShape shape)
{
	Geometry *geo = GeometryLoader::loadGeometry(geoFile);
	Material *mat = MaterialSystem::getSingletonRef().getMaterial(matFile);
	if (!mat)
	{
		mat = MaterialSystem::getSingletonRef().loadMaterial(matFile);
		if (mat)
			MaterialSystem::getSingletonRef().registerMaterial(mat, matFile);
		else
		{
			delete geo;
			return 0;
		}
	}

	return new TemplateMesh(geo, mat, gMat, flags, shape);
}