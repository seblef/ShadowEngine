
#include "stdafx.h"
#include "MapTextLoader.h"
#include "GeometryLoader.h"
#include "GameMesh.h"
#include "GameParticles.h"
#include "GameLight.h"
#include "GameSound.h"
#include "GameEntity.h"
#include "GameCharacter.h"
#include "GameSystem.h"
#include "GroundBinLoader.h"

extern ofstream g_MediaOut;

const char* const g_LightTypes[Light::LT_COUNT]=
{
	_T("omni"),
	_T("spot"),
	_T("area")
};

const char* const g_PhysicShapes[PSHAPE_COUNT]=
{
	"box",
	"sphere",
	"capsule",
	"mesh",
	"chull"
};

Map* MapTextLoader::loadMap(const string& mapName)
{
	Map* m=0;
	string token;
	ScriptFile sf(mapName);
	if(!sf.good() || sf.getToken()!=_T("map"))
		return m;

	sf.getToken();		// Map name

	int w=stoi(sf.getToken());
	int h=stoi(sf.getToken());

	m=new Map(w,h);

	token=sf.getToken();
	while(sf.good() && token!=_T("end_map"))
	{
		parseToken(token,sf,*m);
		token=sf.getToken();
	}

	m->finalize();
	return m;
}

void MapTextLoader::parseToken(const string& token, ScriptFile& sf, Map& m)
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
	else if (token == "sound")
		parseSound(sf, m);
	else if (token == "game_material")
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
		p.x=stof(sf.getToken());
		p.y=stof(sf.getToken());
		m.setStartPosition(p);
	}
	else if(token=="ambient_light")
	{
		Color a;
		sf.parseColor(a);
		LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(a);
	}
}

void MapTextLoader::parseMaterial(ScriptFile& sf)
{
	string name(sf.getToken());
	Material *m=MaterialSystem::getSingletonRef().loadMaterial(sf);
	MaterialSystem::getSingletonRef().registerMaterial(m,name);
}

void MapTextLoader::parseRect(ScriptFile& sf, Map& m)
{
	int x,y,w,h;
	Material* mat;

	x=stoi(sf.getToken());
	y=stoi(sf.getToken());
	w=stoi(sf.getToken());
	h=stoi(sf.getToken());
	mat=MaterialSystem::getSingletonRef().getMaterial(sf.getToken());

	m.getGround().paintRect(x,y,w,h,mat);
}

void MapTextLoader::parseGlobalLight(ScriptFile& sf)
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

void MapTextLoader::parseMesh(ScriptFile& sf, Map& m)
{
	string name(sf.getToken());
	string t(sf.getToken());
	string geo, mat;
	GameMaterial* gmat = 0;
	unsigned int flags=0;
	PhysicShape shape=PSHAPE_BOX;

	while(sf.good() && t!=_T("end_mesh"))
	{
		if(t=="geometry")
			geo=sf.getToken();
		else if(t=="material")
			mat=sf.getToken();
		else if(t=="no_collision")
			flags|=OF_NOCOLLISION;
		else if (t == "physic_shape")
		{
			t = sf.getToken();
			for (int i = 0; i < PSHAPE_COUNT; ++i)
			if (t == g_PhysicShapes[i])
				shape = (PhysicShape)i;
		}
		else if (t == "game_material")
			gmat = m.getGameMaterialDB().getData(sf.getToken());

		t=sf.getToken();
	}

	if(!geo.empty() && !mat.empty())
		m.getMeshDB().registerData(name,TemplateMesh::buildMesh(geo,mat,gmat,flags,shape));
}

void MapTextLoader::parseParticles(ScriptFile& sf, Map& m)
{
	string name(sf.getToken());
	ParticleSystemTemplate *t = new ParticleSystemTemplate(sf);
	TemplateParticleSystem *tps = new TemplateParticleSystem(t);

	m.getParticleDB().registerData(name, tps);
}

void MapTextLoader::parseSound(ScriptFile& sf, Map& m)
{
	string name(sf.getToken());
	string file;
	string t(sf.getToken());
	ISoundCone* cone = 0;
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

	ISound* snd = SoundSystem::getSingletonRef().loadSound(file);
	TemplateSound* tsnd = new TemplateSound(snd, radius, radiusAngle, priority, cone);

	m.getSoundDB().registerData(name, tsnd);
}

void MapTextLoader::parseGameMaterial(ScriptFile& sf, Map& m)
{
	string name(sf.getToken());
	TemplateMesh *debris = 0;

	string t(sf.getToken());
	while (sf.good() && t != "end_game_material")
	{
		if (t == "debris")
			debris = m.getMeshDB().getData(sf.getToken());

		t = sf.getToken();
	}

	m.getGameMaterialDB().registerData(name, new GameMaterial(debris));
}

ISoundCone* MapTextLoader::parseSoundCone(ScriptFile& sf)
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

	return SoundSystem::getSingletonRef().getAudio()->createCone(iAngle, oAngle, iVol, oVol, iLPF, oLPF, iRev,oRev);
}

void MapTextLoader::parseObject(ScriptFile& sf, Map& m)
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

void MapTextLoader::parseMeshObject(ScriptFile& sf, Map& m)
{
	TemplateMesh *tm=m.getMeshDB().getData(sf.getToken());
	if (!tm)
	{
		g_MediaOut << "Failed getting mesh template" << endl;
		return;
	}

	float x=0,y=0,height=0;
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_object"))
	{
		if(t==_T("position"))
		{
			x=stof(sf.getToken());
			y=stof(sf.getToken());
		}
		else if(t==_T("rotation"))
			sf.parseVector(rot);
		else if(t=="height")
			height=stof(sf.getToken());

		t=sf.getToken();
	}

	buildMatrix(rot, x, y, height == 0 ? -tm->getGeometry().getBBox().getMin().y : height, world);
	m.addObject(new GameMesh(*tm,world),false);
}

void MapTextLoader::parseActor(ScriptFile& sf, Map& m)
{
	Character *c = GameSystem::getSingletonRef().getCharacterDB().getData(sf.getToken());
	if (!c)
		return;

	Vector2 pos(Vector2::NullVector);
	float orientation = 0;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == _T("position"))
		{
			pos.x = stof(sf.getToken());
			pos.y = stof(sf.getToken());
		}
		else if (t == _T("rotation"))
			orientation = stof(sf.getToken());

		t = sf.getToken();
	}

	GameCharacter* gc = new GameCharacter(*c, pos, orientation, true);
	m.addObject(gc, true);
}

void MapTextLoader::parseEntity(ScriptFile& sf, Map& m)
{
	EntityTemplate* et = GameSystem::getSingletonRef().getEntityDB().getData(sf.getToken());
	if (!et)
		return;

	Vector3 pos(Vector3::NullVector);
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == _T("position"))
			sf.parseVector(pos);
		else if (t == _T("rotation"))
			sf.parseVector(rot);

		t = sf.getToken();
	}

	buildMatrix(rot, pos.x,pos.z,pos.y,world);
	m.addObject(et->createInstance(world),true);
}

void MapTextLoader::parseParticlesObject(ScriptFile& sf, Map& m)
{
	TemplateParticleSystem* tps = m.getParticleDB().getData(sf.getToken());
	if (!tps)
		return;

	float x = 0, y = 0, height = 0;
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == _T("position"))
		{
			x = stof(sf.getToken());
			y = stof(sf.getToken());
		}
		else if (t == _T("rotation"))
			sf.parseVector(rot);
		else if (t == "height")
			height = stof(sf.getToken());

		t = sf.getToken();
	}

	buildMatrix(rot, x, y, height, world);
	m.addObject(new GameParticles(*tps, world),false);
}

void MapTextLoader::parseSoundObject(ScriptFile& sf, Map& m)
{
	TemplateSound* ts = m.getSoundDB().getData(sf.getToken());
	if (!ts)
		return;

	float x = 0, y = 0, height = 0;
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while (sf.good() && t != _T("end_object"))
	{
		if (t == _T("position"))
		{
			x = stof(sf.getToken());
			y = stof(sf.getToken());
		}
		else if (t == _T("rotation"))
			sf.parseVector(rot);
		else if (t == "height")
			height = stof(sf.getToken());

		t = sf.getToken();
	}

	buildMatrix(rot, x, y, height, world);
	m.addObject(new GameSound(ts, world),false);

}

void MapTextLoader::parseLight(ScriptFile& sf, Map& m)
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

	m.addObject(new GameLight(lc,type),false);
}

void MapTextLoader::parseGround(ScriptFile& sf, Map& m) const
{
	GroundBinLoader loader;
	loader.loadGround(m.getGround(),sf.getToken(),m);
}