
#ifndef _G_RESOURCECOLLECTOR_H_
#define _G_RESOURCECOLLECTOR_H_

#include "G_IMapVisitor.h"
#include "G_MeshTemplate.h"
#include "G_ParticlesTemplate.h"
#include "G_SoundTemplate.h"
#include "G_GameMaterial.h"

class G_ResourceCollector : public G_IMapVisitor
{
public:

	typedef set<Material*>				MaterialSet;
	typedef set<G_MeshTemplate*>		MeshSet;
	typedef set<G_ParticlesTemplate*>	ParticleSet;
	typedef set<G_SoundTemplate*>		SoundSet;
	typedef set<G_GameMaterial*>		GameMaterialSet;

protected:

	MaterialSet				_materials;
	MeshSet					_meshes;
	ParticleSet				_particles;
	SoundSet				_sounds;
	GameMaterialSet			_gameMaterials;

public:

	G_ResourceCollector()				{}

	void					visitGround(G_Ground* g);
	void					visitObject(G_Object* o);

	const MaterialSet&		getMaterials() const			{ return _materials; }
	const MeshSet&			getMeshes() const				{ return _meshes; }
	const ParticleSet&		getParticles() const			{ return _particles;  }
	const SoundSet&			getSounds() const				{ return _sounds; }
	const GameMaterialSet&	getGameMaterials() const		{ return _gameMaterials; }
};

#endif