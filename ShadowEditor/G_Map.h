
#ifndef _G_MAP_H_
#define _G_MAP_H_

#include "G_IMapVisitor.h"
#include "G_MeshTemplate.h"
#include "G_ParticlesTemplate.h"
#include "G_SoundTemplate.h"
#include "G_GameMaterial.h"

class G_Map
{
protected:

	typedef set<G_Object*>		ObjectSet;

	string						_name;
	G_Ground*					_ground;
	Vector2						_startPosition;

	TDataBase<G_MeshTemplate>	_meshes;
	TDataBase<G_ParticlesTemplate>	_particles;
	TDataBase<G_SoundTemplate>	_sounds;
	TDataBase<G_GameMaterial>	_gameMaterials;

	ObjectSet					_objects;

public:

	G_Map(int w, int h);
	~G_Map();

	void								setName(const string& n)	{ _name=n; }
	const string&						getName() const				{ return _name; }

	void								setStartPosition(const Vector2& sp)		{ _startPosition=sp; }
	const Vector2&						getStartPosition() const				{ return _startPosition; }

	G_Ground&							getGround()					{ return *_ground; }
	const G_Ground&						getGround() const			{ return *_ground; }

	TDataBase<G_MeshTemplate>&			getMeshes()					{ return _meshes; }
	const TDataBase<G_MeshTemplate>&	getMeshes() const			{ return _meshes; }

	TDataBase<G_ParticlesTemplate>&		getParticles()				{ return _particles; }
	const TDataBase<G_ParticlesTemplate>&	getParticles() const	{ return _particles; }

	TDataBase<G_SoundTemplate>&			getSounds()					{ return _sounds; }
	const TDataBase<G_SoundTemplate>&	getSounds() const			{ return _sounds; }

	TDataBase<G_GameMaterial>&			getGameMaterials()			{ return _gameMaterials; }
	const TDataBase<G_GameMaterial>&	getGameMaterials() const	{ return _gameMaterials; }

	void								addObject(G_Object* o)
	{
		o->onAddToScene();
		_objects.insert(o);
	}
	void								remObject(G_Object* o)
	{
		o->onRemFromScene();
		_objects.erase(o);
	}

	void								visit(G_IMapVisitor* v);
};

#endif