
#include "stdafx.h"
#include "G_EditorApp.h"
#include "G_Camera.h"
#include "G_ToolSystem.h"
#include "G_MapLoader.h"
#include "G_Selection.h"
#include "G_EditorRenderer.h"
#include "resource.h"
#include "MapPropertiesDlg.h"
#include "../Game/Ground.h"
#include <StdRenderer.h>
#include <DX11MediaMulti.h>
#include <R2D.h>

ofstream	g_MediaOut("Editor.log");

G_EditorApp::G_EditorApp(HINSTANCE hInst) : _mainView(0), _refreshing(false), _config("ShadowEditor.ini")
{
	initShadowCore();

	FileSystemFactory::createFileSystem("std");

	string configFile;
	_config.getVar("configFile", configFile);
	_config.parseFile(configFile);

	_media = createDX11MediaMulti(hInst);
	new Renderer(_media->getVideo(),_config);
	new Renderer2D(_media->getVideo());

	LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(Color::White,-Vector3::YAxisVector);

	new G_ToolSystem;
	new G_Selection;
	new G_EditorRenderer(_media->getVideo());

	_gridRenderer=new G_GridRenderer(_media->getVideo());
	_flagRenderer=new G_FlagRenderer(_media->getVideo());
	_flagRenderer->setFlag(GFLAG_WAKABLE);

	_objDrawer=new G_EditorObjDrawer(_media->getVideo());

	_map=new G_Map(50,50);
	new G_Camera;
	G_Camera::getSingletonRef().initialize(50,50);

	_characters = new CharacterDB("Game/Characters.txt");
	_entities = new G_EntityDB("Game/Entities.txt");
}

G_EditorApp::~G_EditorApp()
{
	G_ToolSystem::deleteSingleton();

	delete _gridRenderer;
	delete _flagRenderer;
	delete _objDrawer;
	delete _map;
	delete _characters;
	delete _entities;

	G_EditorRenderer::deleteSingleton();
	G_Selection::deleteSingleton();
	G_Camera::deleteSingleton();
	Renderer2D::deleteSingleton();
	Renderer::deleteSingleton();
	delete _media;
}

void G_EditorApp::refresh()
{
	if(_refreshing)			return;

	_refreshing=true;

	G_Selection::getSingletonRef().refresh();

	if(_mainView)			getVideo()->setCurrentView(_mainView);

	Renderer::getSingletonRef().update(0,&G_Camera::getSingletonRef().getCamera());
	_gridRenderer->render();
	if(G_Selection::getSingletonRef().isGroundSelected())
		_flagRenderer->render();

	G_EditorRenderer::getSingletonRef().render();

	_objDrawer->begin();
	G_Selection::getSingletonRef().drawSelection(_objDrawer);
	G_ToolSystem::getSingletonRef().draw(_objDrawer);
	_objDrawer->end();

	Renderer2D::getSingletonRef().render();
	_media->getVideo()->newFrame();

	_refreshing=false;
}

void G_EditorApp::loadMap(const string& mapFile)
{
	G_Selection::getSingletonRef().clearSelection();
	G_Selection::getSingletonRef().clearSavedSelection();

	MaterialSystem::getSingletonRef().clearMaterials();
	delete _map;
	G_MapLoader loader;
	_map=loader.loadMap(mapFile);

	refresh();
}

void G_EditorApp::newMap()
{
	CMapPropertiesDlg dlg(_map,false);
	if(dlg.DoModal()==IDCANCEL)
		return;

	G_Selection::getSingletonRef().clearSelection();
	G_Selection::getSingletonRef().clearSavedSelection();

	MaterialSystem::getSingletonRef().clearMaterials();

	delete _map;
	_map=new G_Map(dlg.getWidth(),dlg.getHeight());
	_map->setName(dlg.getName());

	if(dlg.isSunEnable())
		LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(dlg.getSunColor(),dlg.getSunDirection());
	else
		LightSystem::getSingletonRef().getGlobalLight().disableGlobalLight();

	refresh();
}