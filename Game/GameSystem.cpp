
#include "stdafx.h"
#include "GameSystem.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "MapLoaderFactory.h"
#include "ActionServer.h"
#include "SystemValues.h"
#include "GameActorUpdater.h"
#include "EntityUpdater.h"
#include "PhysicEventProcessor.h"
#include "HUD.h"
#include <PhysicLib.h>
#include <NavigationLib.h>
#include <AILib.h>
#include <ctime>

extern ofstream g_MediaOut;

GameSystem::GameSystem(IMedia *media, const string& mapName, const string& playerName, Config& cfg,
	CharacterDB& cdb, WeaponDB& wdb, EntityDB& edb, EffectDB& efdb) : _media(media),
	_characters(&cdb), _weapons(&wdb), _entities(&edb), _effects(&efdb), _gameTime(0), _refreshTime(0)
{
	new ActionServer;
	new Physic(0.02f,Vector3(0,-9.81f,0),true);
	new Navigation;
	new SystemValues;
	new AISystem;
	new GameActorUpdater;
	new EntityUpdater;
	new HUD;

	SystemValues::getSingletonRef().parseConfig(cfg);

#ifdef _DEBUG
	new NavRenderer(Renderer::getSingletonRef().getVideoDevice());
#endif

	IMapLoader* loader=MapLoaderFactory::createMapLoader("text");
	_map=loader->loadMap(mapName);
	delete loader;

	_player=new GamePlayer(*cdb.getData(playerName),_map->getStartPosition());
//	_player->onAddToScene();

	_camera = new GameCamera(_player,
		SystemValues::getSingletonRef().getCameraBaseDistance(),
		Vector2(70, 0),
		(float)media->getVideo()->getResWidth(),
		(float)media->getVideo()->getResHeight());
}

GameSystem::~GameSystem()
{
	ActionServer::getSingletonRef().reset();
	SoundSystem::getSingletonRef().reset();

	delete _player;
	delete _camera;
	delete _map;

	_effects->unloadEffects();
	_entities->unloadEntities();
	_weapons->unloadWeapons();
	_characters->unloadCharacters();

#ifdef _DEBUG
	NavRenderer::deleteSingleton();
#endif

	HUD::deleteSingleton();
	EntityUpdater::deleteSingleton();
	GameActorUpdater::deleteSingleton();
	AISystem::deleteSingleton();
	Physic::deleteSingleton();
	Navigation::deleteSingleton();
	ActionServer::deleteSingleton();
	SystemValues::deleteSingleton();
}

bool GameSystem::update()
{
	static clock_t lastUpdate=clock();
	clock_t now=clock();

	float time=((float)(now-lastUpdate)) / ((float)CLOCKS_PER_SEC);

	if (time > 0.5f)
		time = 0.5f;

	_refreshTime += time;
	if (_refreshTime < 0.01f)
		return false;

	_refreshTime = 0;

	Physic::getSingletonRef().update(time);
	PhysicEventProcessor::processEvents();
	EntityUpdater::getSingletonRef().updateEntities(time);
	_camera->update(time);
	SoundSystem::getSingletonRef().update(time);
	_media->update();

	_media->getVideo()->newFrame();
	_media->getVideo()->resetRenderTargets();
	Renderer::getSingletonRef().update(time,&_camera->getCamera());

#ifdef _DEBUG
	NavRenderer::getSingletonRef().renderNavMap();
#endif

	HUD::getSingletonRef().updateHUD(*_camera);

	Renderer2D::getSingletonRef().render();

	bool end = false;
	Event e;
	while (EventPump::getSingletonRef().getEvent(e))
	{
		if (e._type == ET_CLOSE || (e._type == ET_KEYDOWN && e._param1 == DIK_ESCAPE))
			end = true;
		else if (e._type == ET_KEYDOWN)
			_player->onKeyDown(e._param1);
		else if (e._type == ET_KEYUP)
			_player->onKeyUp(e._param1);
		else if (e._type == ET_MOUSEMOVE)
		{
			_player->setDesiredRotation((float)e._param1);
			_player->setDesiredSpineRotation(-0.25f * (float)e._param2);
		}
		else if (e._type == ET_MOUSEWHEEL)
			_camera->setDistance(_camera->getDistance() * (e._param1 < 0.0f ? 1.1f : 1.0f / 1.1f));
		else if (e._type == ET_MOUSEBUTTONDOWN && e._param1 == 0)
			_player->setFire(true);
		else if (e._type == ET_MOUSEBUTTONUP && e._param1 == 0)
			_player->setFire(false);

	}

	_gameTime += time;
	_player->update(time);

	ActionServer::getSingletonRef().update(time);
	GameActorUpdater::getSingletonRef().updateActors(time);

	AISystem::getSingletonRef().updateAI(_player->getAgentPosition(), _player->getAgentDirection(), time);

	lastUpdate=now;
	return end;
}
