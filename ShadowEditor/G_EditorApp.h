
#ifndef _G_EDITORAPP_H_
#define _G_EDITORAPP_H_

#include <MediaCommon.h>
#include <GameLib.h>
#include "G_Map.h"
#include "G_GridRenderer.h"
#include "G_FlagRenderer.h"
#include "G_EditorObjDrawer.h"
#include "G_EntityDB.h"

using namespace Core;

class G_EditorApp : public TSingleton<G_EditorApp>
{
protected:

	IMedia*						_media;
	IView*						_mainView;

	G_Map*						_map;

	G_GridRenderer*				_gridRenderer;
	G_FlagRenderer*				_flagRenderer;
	G_EditorObjDrawer*			_objDrawer;

	CharacterDB*				_characters;
	G_EntityDB*					_entities;

	bool						_refreshing;

	Config						_config;

public:

	G_EditorApp(HINSTANCE hinst);
	~G_EditorApp();

	IMultiVideoDevice*			getVideo() const						{ return (IMultiVideoDevice*)_media->getVideo(); }
	ISoundDevice*				getAudio() const						{ return _media->getAudio(); }

	void						setMainView(IView* v)					{ _mainView=v; }
	void						refresh();

	void						newMap();
	void						loadMap(const string& mapFile);
	G_Map&						getMap()								{ return *_map; }

	CharacterDB&				getCharacterDB() const					{ return *_characters; }
	G_EntityDB&					getEntityDB() const						{ return *_entities; }
};

#endif