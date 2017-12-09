
#include "stdafx.h"
#include "G_MapSaver.h"
#include "G_ResourceCollector.h"
#include "G_MaterialSaver.h"
#include "G_MeshTemplateSaver.h"
#include "G_ParticlesTemplateSaver.h"
#include "G_SoundTemplateSaver.h"
#include "G_GameMaterialSaver.h"
#include "G_GroundSaver.h"
#include "G_Mesh.h"
#include "G_ParticleSystem.h"
#include "G_Light.h"
#include "G_Sound.h"
#include "G_Actor.h"
#include "G_Entity.h"

void G_MapSaver::saveMap()
{
	string fileName("Maps/");
	fileName+=_map->getName();
	fileName+=".txt";

	ofstream ofs(fileName,fstream::out);
	_ofs=&ofs;

	G_ResourceCollector col;
	_map->visit(&col);

	ofs << "map " << _map->getName() << " " << _map->getGround().getWidth() << " " << _map->getGround().getHeight() << endl << endl;

	const LightGlobal& b(LightSystem::getSingletonRef().getGlobalLight());
	if(b.isGlobalLightEnable())
	{
		const Vector4& d(b.getDirection());
		const Color& c(b.getColor());

		ofs << "global_light" << endl;
		ofs << "	dir " << d.x << " " << d.y << " " << d.z << endl;
		ofs << "	color " << c.r << " " << c.g << " " << c.b << " " << c.a << endl << "	end_global_light" << endl << endl;
	}

	ofs << "ambient_light " << b.getAmbientLight().r << " " << b.getAmbientLight().g << " " << b.getAmbientLight().b << " " << b.getAmbientLight().a << endl << endl;

	ofs << "start_position " << _map->getStartPosition().x << " " << _map->getStartPosition().y << endl << endl;

	G_MaterialSaver matSave;
	matSave.saveMaterials(col,ofs);

	G_MeshTemplateSaver meshSaver;
	meshSaver.saveMeshes(col,ofs,false);

	G_ParticlesTemplateSaver particleSaver;
	particleSaver.saveParticleSystems(col, ofs);

	G_SoundTemplateSaver soundSaver;
	soundSaver.saveSounds(col, ofs);

	G_GameMaterialSaver gameMatSaver;
	gameMatSaver.saveMaterials(col, ofs);

	meshSaver.saveMeshes(col, ofs, true);

	_map->visit(this);

	ofs << "end_map" << endl;
}

void G_MapSaver::visitGround(G_Ground* g)
{
	string file("Maps/");
	file+=_map->getName();
	file+=".sgd";

	(*_ofs) << endl << "ground " << file << endl << endl;

	G_GroundSaver saver;
	saver.saveGround(*g,file);
}

void G_MapSaver::visitObject(G_Object *o)
{
	switch(o->getType())
	{
	case G_Object::MESH:			saveMesh(o);			break;
	case G_Object::LIGHT:			saveLight(o);			break;
	case G_Object::PARTICLESYSTEM:	saveParticleSystem(o);	break;
	case G_Object::SOUND:			saveSound(o);			break;
	case G_Object::ACTOR:			saveActor(o);			break;
	case G_Object::ENTITY:			saveEntity(o);			break;
	}
}

void G_MapSaver::saveMesh(G_Object *o) const
{
	G_Mesh *m=(G_Mesh*)o;
	(*_ofs) << "object mesh " << m->getTemplate()->getName() << endl << endl;
	(*_ofs) << "	position " << o->getPosition().x << " " << o->getPosition().z << endl;
	(*_ofs) << "	rotation " << o->getRotation().x << " " << o->getRotation().y << " " << o->getRotation().z << endl;

	if(o->getPosition().y!=o->getLocalBox().getMin().y)
		(*_ofs) << "	height " << o->getPosition().y << endl;

	(*_ofs) << "end_object" << endl << endl;
}

void G_MapSaver::saveLight(G_Object* o) const
{
	G_Light* gl=(G_Light*)o;
	const Light& l=gl->getLight();

	(*_ofs) << "light " << g_LightTypes[l.getLightType()] << endl;
	(*_ofs) << "	color "; outputColor(l.getColor()); (*_ofs) << endl;
	(*_ofs) << "	range " << l.getRange() << endl;
	outputVector("position",l.getWorldMatrix());

	if(l.getCastShadows())
	{
		(*_ofs) << "	shadowmap_size " << l.getShadowMapSize() << endl;
		(*_ofs) << "	shadowmap_filter " << l.getShadowMapFilter() << endl;
	}
	else
		(*_ofs) << "	cast_no_shadows" << endl;

	if(l.getLightType()==Light::LT_SPOT)
	{
		const LightSpot *s=(const LightSpot*)&l;
		outputVector("direction",s->getDirection());
		(*_ofs) << "	far_angle " << s->getFarAngle() << endl;
		(*_ofs) << "	near_angle " << s->getNearAngle() << endl;
		(*_ofs) << "	near_z " << s->getNearZ() << endl;
	}
	else if(l.getLightType()==Light::LT_AREA)
	{
		const LightArea *a=(const LightArea*)&l;
		outputVector("direction",a->getDirection());
		(*_ofs) << "	far_angle " << a->getFarAngle() << endl;
		(*_ofs) << "	near_angle " << a->getNearAngle() << endl;
		(*_ofs) << "	near_z " << a->getNearZ() << endl;
		(*_ofs) << "	area_width " << a->getAreaWidth() << endl;
		(*_ofs) << "	area_height " << a->getAreaHeight() << endl;
	}

	(*_ofs) << "end_light" << endl << endl;

}

void G_MapSaver::saveParticleSystem(G_Object *o) const
{
	G_ParticleSystem *ps = (G_ParticleSystem*)o;
	(*_ofs) << "object particles " << ps->getTemplate()->getName() << endl << endl;
	(*_ofs) << "	position " << o->getPosition().x << " " << o->getPosition().z << endl;
	(*_ofs) << "	rotation " << o->getRotation().x << " " << o->getRotation().y << " " << o->getRotation().z << endl;
	(*_ofs) << "	height " << o->getPosition().y << endl << endl;

	(*_ofs) << "end_object" << endl << endl;
}

void G_MapSaver::saveSound(G_Object *o) const
{
	G_Sound *ps = (G_Sound*)o;
	(*_ofs) << "object sound " << ps->getTemplate()->getName() << endl << endl;
	(*_ofs) << "	position " << o->getPosition().x << " " << o->getPosition().z << endl;
	(*_ofs) << "	rotation " << o->getRotation().x << " " << o->getRotation().y << " " << o->getRotation().z << endl;
	(*_ofs) << "	height " << o->getPosition().y << endl << endl;

	(*_ofs) << "end_object" << endl << endl;
}

void G_MapSaver::saveActor(G_Object *o) const
{
	G_Actor* a = (G_Actor*)o;
	(*_ofs) << "object actor " << a->getTemplate()->getName() << endl << endl;
	(*_ofs) << "	position " << o->getPosition().x << " " << o->getPosition().z << endl;
	(*_ofs) << "	rotation " << o->getRotation().y  << endl << endl;

	(*_ofs) << "end_object" << endl << endl;

}

void G_MapSaver::saveEntity(G_Object* o) const
{
	G_Entity* e = (G_Entity*)o;
	(*_ofs) << "object entity " << e->getTemplate()->getName() << endl << endl;
	(*_ofs) << "	position " << o->getPosition().x << " " << o->getPosition().y << " " << o->getPosition().z << endl;
	(*_ofs) << "	rotation " << o->getRotation().x << " " << o->getRotation().y << " " << o->getRotation().z << endl << endl;
	(*_ofs) << "end_object" << endl << endl;
}