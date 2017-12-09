
#ifndef _G_ENTITYTEMPLATE_H_
#define _G_ENTITYTEMPLATE_H_

#include <StdRenderer.h>


class G_EntityTemplate
{
protected:

	string				_name;
	string				_meshFile;
	string				_materialFile;

	Geometry*			_geo;
	Mesh*				_rMesh;
	Material*			_rMaterial;
	void*				_physic;

public:

	G_EntityTemplate(ScriptFile& sf);
	~G_EntityTemplate();

	void				load();

	const string&		getName() const				{ return _name; }

	Geometry*			getGeometry() const			{ return _geo; }
	Mesh*				getMesh() const				{ return _rMesh; }
	Material*			getMaterial() const			{ return _rMaterial; }
	void*				getPhysicMesh() const		{ return _physic; }

};

#endif