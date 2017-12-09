
#ifndef _G_CREATOR_H_
#define _G_CREATOR_H_

#include "G_MeshTemplate.h"
#include "G_ParticlesTemplate.h"
#include "G_SoundTemplate.h"
#include "G_EntityTemplate.h"

class G_Creator
{
public:

	G_Creator()					{}

	static void					createMesh(G_MeshTemplate* t);
	static void					createActor(Character* c);
	static void					createParticleSystem(G_ParticlesTemplate *t);
	static void					createLight(Light::LightType t);
	static void					createSound(G_SoundTemplate *t);
	static void					createEntity(G_EntityTemplate* t);
};

#endif