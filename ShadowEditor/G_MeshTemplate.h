
#ifndef _G_MESHTEMPLATE_H_
#define _G_MESHTEMPLATE_H_

#include <GameLib.h>
#include "../Collision/PhysicDefs.h"

class G_GameMaterial;

class G_MeshTemplate
{
protected:

	Geometry*					_geo;
	Material*					_mat;
	G_GameMaterial*				_gameMaterial;

	string						_name;
	string						_geoFile;
	string						_matName;
	PhysicShape					_shape;
	unsigned int				_flags;

	Mesh*						_rMesh;
	void*						_sMesh;

public:

	G_MeshTemplate(const string& name, const string& geoFile, const string& matName,
		PhysicShape ps, unsigned int flags);
	~G_MeshTemplate();

	void						setName(const string& n)		{ _name=n; }
	const string&				getName() const					{ return _name; }

	Material&					getMaterial()					{ return *_mat; }
	const Material&				getMaterial() const				{ return *_mat; }
	const string&				getMaterialName() const			{ return _matName; }
	void						setMaterial(const string& mat);

	G_GameMaterial*				getGameMaterial() const			{ return _gameMaterial; }
	void						setGameMaterial(G_GameMaterial* m)	{ _gameMaterial = m; }

	const string&				getGeometryFile() const			{ return _geoFile; }
	Geometry*					getGeometry() const				{ return _geo; }

	PhysicShape					getPhysicShape() const			{ return _shape; }
	void						setPhysicShape(PhysicShape ps)	{ _shape=ps; }

	void						setFlag(unsigned int f)			{ _flags|=f; }
	void						unsetFlag(unsigned int f)		{ _flags&= ~f; }
	unsigned int				getFlag(unsigned int f)	const	{ return _flags & f; }

	Mesh*						getMesh() const					{ return _rMesh; }
	void*						getSelectionMesh() const		{ return _sMesh; }
};

#endif