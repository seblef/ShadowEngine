
#include "stdafx.h"
#include "G_MapLoader.h"
#include "G_EditorApp.h"
#include "G_GroundLoader.h"
#include "G_Mesh.h"
#include "G_Light.h"
#include "G_ParticleSystem.h"
#include "G_Sound.h"
#include "G_Actor.h"
#include "G_Entity.h"
#include "G_GameMaterial.h"
#include "../Game/GameObject.h"

G_Map* G_MapLoader::loadMap(const string& mapFile) const
{
	G_Map* m=0;
	string token;
	ScriptFile sf(mapFile);
	if(!sf.good() || sf.getToken()!=_T("map"))
		return m;

	string mapName(sf.getToken());

	int w=stoi(sf.getToken());
	int h=stoi(sf.getToken());

	m=new G_Map(w,h);
	m->setName(mapName);

	token=sf.getToken();
	while(sf.good() && token!=_T("end_map"))
	{
		parseToken(token,sf,*m);
		token=sf.getToken();
	}

	return m;
}

void G_MapLoader::parseToken(const string& token, ScriptFile& sf, G_Map& m) const
{
	if(token==_T("material"))
		parseMaterial(sf);
	else if(token==_T("rect"))
		parseRect(sf,m);
	else if(token==_T("global_light"))
		parseGlobalLight(sf);
	else if (token == _T("mesh"))
		parseMesh(sf, m);
	else if (token == _T("particle_system"))
		parseParticles(sf, m);
	else if (token == _T("sound"))
		parseSound(sf, m);
	else if (token == _T("game_material"))
		parseGameMaterial(sf, m);
	else if(token==_T("object"))
		parseObject(sf,m);
	else if(token==_T("light"))
		parseLight(sf,m);
	else if(token=="ground")
		parseGround(sf,m);
	else if(token==_T("start_position"))
	{
		Vector2 p;
		sf.parseVector(p);
		m.setStartPosition(p);
	}
	else if(token=="ambient_light")
	{
		Color a;
		sf.parseColor(a);
		LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(a);
	}
}

void G_MapLoader::parseMaterial(ScriptFile& sf) const
{
	string name(sf.getToken());
	Material *m=MaterialSystem::getSingletonRef().loadMaterial(sf);
	MaterialSystem::getSingletonRef().registerMaterial(m,name);
}

void G_MapLoader::parseRect(ScriptFile& sf, G_Map& m) const
{
	int x,y,w,h;
	Material* mat;

	x=stoi(sf.getToken());
	y=stoi(sf.getToken());
	w=stoi(sf.getToken());
	h=stoi(sf.getToken());
	mat=MaterialSystem::getSingletonRef().getMaterial(sf.getToken());
}

void G_MapLoader::parseGlobalLight(ScriptFile& sf) const
{
	Color c(0.9,0.8,0.8,1.0);
	Vector3 d(0,-1,0);

	string t=sf.getToken();
	while(sf.good() && t!=_T("end_global_light"))
	{
		if(t==_T("dir"))
			sf.parseVector(d);
		else if(t==_T("color"))
			sf.parseColor(c);

		t=sf.getToken();
	}

	d.normalize();
	LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(c,d);
}

void G_MapLoader::parseMesh(ScriptFile& sf, G_Map& m) const
{
	string name(sf.getToken());
	string t(sf.getToken());
	string geo;
	string mat;
	unsigned int flags=0;
	PhysicShape shape=PSHAPE_BOX;
	G_GameMaterial* gmat = 0;

	while(sf.good() && t!=_T("end_mesh"))
	{
		if(t==_T("geometry"))
			geo=sf.getToken();
		else if(t==_T("material"))
			mat=sf.getToken();
		else if(t==_T("no_collision"))
			flags|=OF_NOCOLLISION;
		else if (t == _T("physic_shape"))
		{
			t = sf.getToken();
			for (int i = 0; i<PSHAPE_COUNT; ++i)
			if (t == g_PhysicShapeNames[i])
				shape = (PhysicShape)i;
		}
		else if (t == "game_material")
			gmat = m.getGameMaterials().getData(sf.getToken());

		t=sf.getToken();
	}

	if (geo.length() > 0 && mat.length() > 0)
	{
		G_MeshTemplate* mesh = new G_MeshTemplate(name, geo, mat, shape, flags);
		mesh->setGameMaterial(gmat);
		m.getMeshes().registerData(name, mesh);
	}
}

void G_MapLoader::parseParticles(ScriptFile& sf, G_Map& m) const
{
	string name(sf.getToken());
	ParticleSystemTemplate *t = new ParticleSystemTemplate(sf);
	G_ParticlesTemplate *tps = new G_ParticlesTemplate(t,name);

	m.getParticles().registerData(name, tps);
}

void G_MapLoader::parseSound(ScriptFile& sf, G_Map& m) const
{
	string name(sf.getToken());
	string file;
	string t(sf.getToken());
	G_SoundCone* cone = 0;
	float radius = 10.0f, radiusAngle = 0.0f;
	int priority = 1;

	while (sf.good() && t != _T("end_sound"))
	{
		if (t == "file")
			file = sf.getToken();
		else if (t == "priority")
			priority = stoi(sf.getToken());
		else if (t == "radius")
			radius = stof(sf.getToken());
		else if (t == "radius_angle")
			radiusAngle = stof(sf.getToken());
		else if (t == "cone")
			cone = parseSoundCone(sf);

		t = sf.getToken();
	}

	G_SoundTemplate* snd = new G_SoundTemplate(name, priority, radius, radiusAngle, file, cone);
	m.getSounds().registerData(name, snd);
}

void G_MapLoader::parseGameMaterial(ScriptFile& sf, G_Map& m) const
{
	string name(sf.getToken());
	string t(sf.getToken());

	G_MeshTemplate* debris = 0;

	while (sf.good() && t != "end_game_material")
	{
		if (t == "debris")
			debris = m.getMeshes().getData(sf.getToken());

		t = sf.getToken();
	}

	G_GameMaterial* mat = new G_GameMaterial(name, debris);
	m.getGameMaterials().registerData(name, mat);
}

G_SoundCone* G_MapLoader::parseSoundCone(ScriptFile& sf) const
{
	float iAngle = 0.0f;
	float oAngle = 0.0f;
	float iVol = 1.0f;
	float oVol = 0.0f;
	float iLPF = 0.0f;
	float oLPF = 0.0f;
	float iRev = 0.0f;
	float oRev = 0.0f;
	string token(sf.getToken());

	while (sf.good() && token != "end_cone")
	{
		if (token == "in_angle")			iAngle = stof(sf.getToken());
		else if (token == "out_angle")		oAngle = stof(sf.getToken());
		else if (token == "in_volume")		iVol = stof(sf.getToken());
		else if (token == "out_volume")		oVol = stof(sf.getToken());
		else if (token == "in_lpf")			iLPF = stof(sf.getToken());
		else if (token == "out_lpf")		oLPF = stof(sf.getToken());
		else if (token == "in_reverb")		iRev = stof(sf.getToken());
		else if (token == "out_reverb")		oRev = stof(sf.getToken());

		token = sf.getToken();
	}

	return new G_SoundCone(iAngle, oAngle, iVol, oVol, iLPF, oLPF, iRev, oRev);
}

void G_MapLoader::parseObject(ScriptFile& sf, G_Map& m) const
{
	string t(sf.getToken());
	if (t == _T("mesh"))
		parseMeshObject(sf, m);
	else if (t == _T("particles"))
		parseParticlesObject(sf, m);
	else if (t == _T("sound"))
		parseSoundObject(sf, m);
	else if (t == "actor")
		parseActor(sf, m);
	else if (t == "entity")
		parseEntity(sf, m);
}

void G_MapLoader::parseMeshObject(ScriptFile& sf, G_Map& m) const
{
	G_MeshTemplate *tm=m.getMeshes().getData(sf.getToken());
	if (!tm)
		return;

	Vector3 p(Vector3::NullVector), r(Vector3::NullVector);
	float x=0,y=0,height=0;

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_object"))
	{
		if(t==_T("position"))
		{
			p.x=stof(sf.getToken());
			p.z=stof(sf.getToken());
		}
		else if(t==_T("rotation"))
			sf.parseVector(r);
		else if(t=="height")
			height=stof(sf.getToken());

		t=sf.getToken();
	}

	if(height==0)
		p.y=-tm->getGeometry()->getBBox().getMin().y;
	else
		p.y=height;

	G_Mesh* gm=new G_Mesh(tm);
	m.addObject(gm);
	gm->setPosition(p);
	gm->setRotation(r);
}

void G_MapLoader::parseActor(ScriptFile& sf, G_Map& m) const
{
	Character *c = G_EditorApp::getSingletonRef().getCharacterDB().getData(sf.getToken());
	if (!c)
		return;

	Vector3 p(Vector3::NullVector), r(Vector3::NullVector);
	float x = 0, y = 0, height = 0;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == _T("position"))
		{
			p.x = stof(sf.getToken());
			p.z = stof(sf.getToken());
		}
		else if (t == _T("rotation"))
			r.y = stof(sf.getToken());

		t = sf.getToken();
	}

	G_Actor *a = new G_Actor(c);
	m.addObject(a);
	p.y = -c->getActor()->getBBox().getMin().y;
	a->setPosition(p);
	a->setRotation(r);
}

void G_MapLoader::parseEntity(ScriptFile& sf, G_Map& m) const
{
	G_EntityTemplate *et = G_EditorApp::getSingletonRef().getEntityDB().getData(sf.getToken());
	if (!et)
		return;

	Vector3 p(Vector3::NullVector), r(Vector3::NullVector);
	float x = 0, y = 0, height = 0;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == "position")
			sf.parseVector(p);
		else if (t == "rotation")
			sf.parseVector(r);

		t = sf.getToken();
	}

	G_Entity *e = new G_Entity(et);
	m.addObject(e);

	e->setPosition(p);
	e->setRotation(r);
}

void G_MapLoader::parseParticlesObject(ScriptFile& sf, G_Map& m) const
{
	G_ParticlesTemplate *tp = m.getParticles().getData(sf.getToken());
	if (!tp)
		return;

	Vector3 p(Vector3::NullVector), r(Vector3::NullVector);
	float x = 0, y = 0, height = 0;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == _T("position"))
		{
			p.x = stof(sf.getToken());
			p.z = stof(sf.getToken());
		}
		else if (t == _T("rotation"))
			sf.parseVector(r);
		else if (t == "height")
			p.y = stof(sf.getToken());

		t = sf.getToken();
	}

	G_ParticleSystem *ps = new G_ParticleSystem(tp);
	m.addObject(ps);
	ps->setPosition(p);
	ps->setRotation(r);
}

void G_MapLoader::parseSoundObject(ScriptFile& sf, G_Map& m) const
{
	G_SoundTemplate *tp = m.getSounds().getData(sf.getToken());
	if (!tp)
		return;

	Vector3 p(Vector3::NullVector), r(Vector3::NullVector);
	float x = 0, y = 0, height = 0;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == _T("position"))
		{
			p.x = stof(sf.getToken());
			p.z = stof(sf.getToken());
		}
		else if (t == _T("rotation"))
			sf.parseVector(r);
		else if (t == "height")
			p.y = stof(sf.getToken());

		t = sf.getToken();
	}

	G_Sound *ps = new G_Sound(tp);
	m.addObject(ps);
	ps->setPosition(p);
	ps->setRotation(r);
}

void G_MapLoader::parseLight(ScriptFile& sf, G_Map& m) const
{
	string t(sf.getToken());
	Light::LightType type=Light::LT_OMNI;

	for(int i=0;i<Light::LT_COUNT;++i)
		if(t==g_LightTypes[i])
			type=(Light::LightType)i;

	LightCreate_t lc;
	lc._world.createIdentity();
	lc._color=Color::White;
	lc._range=2;
	lc._direction=-Vector3::YAxisVector;
	lc._nearAngle=45;
	lc._farAngle=60;
	lc._areaWidth=2;
	lc._areaHeight=1;
	lc._nearZ=0.1f;
	lc._castShadows=true;
	lc._shadowMapSize=256;
	lc._shadowMapFilter=4;

	t=sf.getToken();
	while(sf.good() && t!=_T("end_light"))
	{
		if(t==_T("color"))					sf.parseColor(lc._color);
		else if(t==_T("range"))				lc._range=stof(sf.getToken());
		else if(t==_T("position"))			sf.parseVector(lc._world);
		else if(t==_T("direction"))			sf.parseVector(lc._direction);
		else if(t==_T("near_angle"))		lc._nearAngle=stof(sf.getToken());
		else if(t==_T("far_angle"))			lc._farAngle=stof(sf.getToken());
		else if(t==_T("area_width"))		lc._areaWidth=stof(sf.getToken());
		else if(t==_T("area_height"))		lc._areaHeight=stof(sf.getToken());
		else if(t==_T("near_z"))			lc._nearZ=stof(sf.getToken());
		else if(t==_T("cast_no_shadows"))	lc._castShadows=false;
		else if(t=="shadowmap_size")		lc._shadowMapSize=stoi(sf.getToken());
		else if(t=="shadowmap_filter")		lc._shadowMapFilter=stoi(sf.getToken());

		t=sf.getToken();
	}

	Light::computeMatrix(lc._direction,lc._world);

	G_Light *l=new G_Light(type,lc);
	m.addObject(l);
	l->setPosition((const Vector3&)lc._world);
}

void G_MapLoader::parseGround(ScriptFile& sf, G_Map& m) const
{
	G_GroundLoader loader;
	loader.loadGround(sf.getToken(),m.getGround(),m);
}