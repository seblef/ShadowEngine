
#ifndef _TEMPLATEMESH_H_
#define _TEMPLATEMESH_H_

#include "Geometry.h"
#include <StdRenderer.h>
#include <PhysicLib.h>

class GameMaterial;

class TemplateMesh
{
protected:

	Geometry*					_geometry;
	Material*					_material;
	GameMaterial*				_gameMaterial;
	Mesh*						_rMesh;
	unsigned int				_flags;

	PhysicGeometry*				_physic;

public:

	TemplateMesh(Geometry* geo, Material* mat, GameMaterial *gmat, unsigned int flags, PhysicShape shape);
	~TemplateMesh();

	unsigned int				getFlags() const					{ return _flags; }
	const Geometry&				getGeometry() const					{ return *_geometry; }
	const Mesh&					getMesh() const						{ return *_rMesh; }
	PhysicGeometry*				getPhysicGeometry() const			{ return _physic; }
	GameMaterial*				getGameMaterial() const				{ return _gameMaterial; }

	static TemplateMesh*		buildMesh(const string& geoFile, const string& matFile,
		GameMaterial *gMat=0, unsigned int flags = 0, PhysicShape shape = PSHAPE_SPHERE);
};

#endif