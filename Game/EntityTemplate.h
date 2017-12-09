
#ifndef _ENTITYTEMPLATE_H_
#define _ENTITYTEMPLATE_H_

#include <StdRenderer.h>
#include <PhysicLib.h>
#include "Geometry.h"

class GameEntity;

class EntityTemplate
{
protected:

	string					_meshName;
	string					_materialName;

	Geometry*				_geometry;
	Mesh*					_rMesh;
	Material*				_rMaterial;
	PhysicGeometry*			_pGeometry;
	PhysicShape				_pShape;

	bool					_isTrigger;

	virtual void			parseToken(const string& token, ScriptFile& sf);

public:

	EntityTemplate(bool isTrigger);
	virtual ~EntityTemplate()		 { unload(); }

	bool					isTrigger() const					{ return _isTrigger; }

	virtual void			load();
	virtual void			unload();

	Mesh*					getMesh() const						{ return _rMesh; }
	Material*				getMaterial() const					{ return _rMaterial; }
	Geometry*				getGeometry() const					{ return _geometry; }
	PhysicGeometry*			getPhysicGeometry() const			{ return _pGeometry; }

	virtual GameEntity*		createInstance(const Matrix4& world) = 0;
};

#endif