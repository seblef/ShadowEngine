
#include "stdafx.h"
#include "GameApp.h"
#include <GameLib.h>
#include <SoundLib.h>
#include <time.h>
#include <sstream>

//#define HDAO_TUNING

#ifdef HDAO_TUNING
#include "../Renderer/HDAO.h"
#endif


extern ofstream g_MediaOut;

void GameApp::run(HINSTANCE hInst)
{
	initShadowCore();

	FileSystemFactory::createFileSystem(_T("std"));
	Config cfg("Shadow.ini");

	string configFile;
	cfg.getVar("configFile", configFile);
	cfg.parseFile(configFile);

	int w = 800;
	int h = 600;
	bool windowed = true;
	bool multithread = true;

	cfg.getVar("resWidth", w);
	cfg.getVar("resHeight", h);
	cfg.getVar("windowed", windowed);
	cfg.getVar("multithread", multithread);

	IMedia *media=createDX11Media(hInst,w,h,windowed,multithread,SINPUT_MOUSE | SINPUT_KEYBOARD);
	new SoundSystem(media->getAudio());
	new Renderer(media->getVideo(),cfg);
	new Renderer2D(media->getVideo());

	CharacterDB *cdb=new CharacterDB("Game/Characters.txt");
	WeaponDB *wdb=new WeaponDB("Game/Weapons.txt");
	EntityDB* edb = new EntityDB("Game/Entities.txt");
	EffectDB* efdb = new EffectDB("Game/Effects.txt");

	string mapName("Maps/SciFy.txt");
	string playerName("player");
	cfg.getVar("map", mapName);
	cfg.getVar("player", playerName);

	new GameSystem(media,mapName,playerName,cfg,*cdb,*wdb,*edb,*efdb);

#ifdef HDAO_TUNING
	R2D_Layer* hdao_layer = new R2D_Layer;
	R2D_Font *fnt = R2D_FontLoader::loadFont("2D/Font.sfn",media->getVideo());

	R2D_Text* hdao_normalScale = new R2D_Text(fnt);
	R2D_Text* hdao_acceptAngle = new R2D_Text(fnt);
	R2D_Text* hdao_acceptRadius = new R2D_Text(fnt);
	R2D_Text* hdao_rejectRadius = new R2D_Text(fnt);
	R2D_Text* hdao_intensity = new R2D_Text(fnt);

	hdao_normalScale->setColor(Color::White);
	hdao_normalScale->setPosition(Vector2(10.0f, 20.0f));
	hdao_normalScale->setSize(Vector2(200, 10));

	hdao_acceptAngle->setColor(Color::White);
	hdao_acceptAngle->setPosition(Vector2(10, 40));
	hdao_acceptAngle->setSize(Vector2(200, 10));

	hdao_acceptRadius->setColor(Color::White);
	hdao_acceptRadius->setPosition(Vector2(10, 60));
	hdao_acceptRadius->setSize(Vector2(200, 10));

	hdao_rejectRadius->setColor(Color::White);
	hdao_rejectRadius->setPosition(Vector2(10, 80));
	hdao_rejectRadius->setSize(Vector2(200, 10));

	hdao_intensity->setColor(Color::White);
	hdao_intensity->setPosition(Vector2(10, 100));
	hdao_intensity->setSize(Vector2(200, 10));

	hdao_layer->getSet().insert(hdao_acceptAngle);
	hdao_layer->getSet().insert(hdao_normalScale);
	hdao_layer->getSet().insert(hdao_acceptRadius);
	hdao_layer->getSet().insert(hdao_rejectRadius);
	hdao_layer->getSet().insert(hdao_intensity);

	Renderer2D::getSingletonRef().getCurrentFrame()->getVector().push_back(hdao_layer);
#endif

	bool end=false;
	while (!end)
	{
		end = GameSystem::getSingletonRef().update();

#ifdef HDAO_TUNING
		hdao_normalScale->setText("NormalScale " + to_string(HDAO::getSingletonRef().getNormalScale()));
		hdao_acceptAngle->setText("AcceptAngle " + to_string(HDAO::getSingletonRef().getAcceptAngle()));
		hdao_acceptRadius->setText("AcceptRadius " + to_string(HDAO::getSingletonRef().getAcceptRadius()));
		hdao_rejectRadius->setText("RejectRadius " + to_string(HDAO::getSingletonRef().getRejectRadius()));
		hdao_intensity->setText("Intensity " + to_string(HDAO::getSingletonRef().getIntensity()));
#endif
	}

	GameSystem::deleteSingleton();

	delete cdb;
	delete wdb;
	delete edb;

	Renderer2D::deleteSingleton();
	Renderer::deleteSingleton();
	SoundSystem::deleteSingleton();
	delete media;
}