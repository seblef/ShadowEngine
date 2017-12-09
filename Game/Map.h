
#ifndef _MAP_H_
#define _MAP_H_

#include "Ground.h"
#include "TemplateMesh.h"
#include "TemplateParticleSystem.h"
#include "TemplateSound.h"
#include "GameObject.h"
#include "GameCharacter.h"
#include "GameMaterial.h"

class Map
{
public:

	typedef TDataBase<TemplateMesh>				MeshDB;
	typedef TDataBase<TemplateParticleSystem>	ParticleDB;
	typedef TDataBase<TemplateSound>			SoundDB;
	typedef TDataBase<GameMaterial>				GameMaterialDB;

protected:

	typedef TVector<GameObject>				ObjectVector;
	typedef TSet<GameObject>				ObjectSet;

	Ground				_ground;
	MeshDB				_meshes;
	ParticleDB			_particles;
	SoundDB				_sounds;
	GameMaterialDB		_gameMaterials;

	GameMaterial		_nullGameMaterial;

	ObjectVector		_staticObjects;
	ObjectSet			_tempObjects;

	Vector2				_startPosition;

	IPhysicObject*		_pObject;

public:

	Map(int w, int h);
	~Map();

	void				setStartPosition(const Vector2& p)		{ _startPosition=p; }
	const Vector2&		getStartPosition() const				{ return _startPosition; }

	Ground&				getGround()						{ return _ground; }
	const Ground&		getGround() const				{ return _ground; }

	void				finalize();

	void				addObject(GameObject* o, bool temp);
	void				remObject(GameObject *o)		{ o->onRemFromScene(); _tempObjects.getSet().erase(o); }

	MeshDB&				getMeshDB()						{ return _meshes; }
	ParticleDB&			getParticleDB()					{ return _particles; }
	SoundDB&			getSoundDB()					{ return _sounds; }
	GameMaterialDB&		getGameMaterialDB()				{ return _gameMaterials; }

	void				outputWakableInfo() const;
};

#endif