
#include "stdafx.h"
#include "G_ResourceCollector.h"
#include "G_Mesh.h"
#include "G_ParticleSystem.h"
#include "G_Sound.h"

void G_ResourceCollector::visitGround(G_Ground* g)
{
	for(int i=0;i<g->getMaterialCount();++i)
		_materials.insert(&g->getMaterial(i));

	for (int i = 1; i < g->getGameMaterialCount(); ++i)
		_gameMaterials.insert(g->getGameMaterial(i));
}

void G_ResourceCollector::visitObject(G_Object* o)
{
	if(o->getType()==G_Object::MESH)
	{
		G_MeshTemplate *t=((G_Mesh*)o)->getTemplate();
		_meshes.insert(t);
		_materials.insert(&t->getMaterial());

		if (t->getGameMaterial())
		{
			_gameMaterials.insert(t->getGameMaterial());
			if (t->getGameMaterial()->getDebris())
			{
				_meshes.insert(t->getGameMaterial()->getDebris());
				_materials.insert(&t->getGameMaterial()->getDebris()->getMaterial());
			}
		}
	}
	else if (o->getType() == G_Object::PARTICLESYSTEM)
	{
		G_ParticlesTemplate* t = ((G_ParticleSystem*)o)->getTemplate();
		_particles.insert(t);
	}
	else if (o->getType() == G_Object::SOUND)
	{
		G_SoundTemplate* t = ((G_Sound*)o)->getTemplate();
		_sounds.insert(t);
	}
}