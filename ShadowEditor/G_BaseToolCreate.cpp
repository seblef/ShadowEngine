
#include "stdafx.h"
#include "G_BaseToolCreate.h"
#include "G_ToolSystem.h"
#include "G_Selection.h"
#include "G_EditorApp.h"

void G_BaseToolCreate::mouseDown(int x, int y, unsigned short flags)
{
	G_Selection::getSingletonRef().select(_object,false);
	_flags=flags;
	
	Vector3 p(Vector3::NullVector);
	getGroundPosition(x,y,p);

	G_EditorApp::getSingletonRef().getMap().addObject(_object);

	p.y=-_box.getMin().y;
	_object->setPosition(p);
	_originals.push_back(p);

	if(_flags & GTOOL_SNAPGRID)
		snap(_object);

	_lastPos=p;
	_lastPos.y=0;
	_translation=Vector3::NullVector;

	G_EditorApp::getSingletonRef().refresh();
}

void G_BaseToolCreate::mouseUp(int x, int y)
{
	G_BaseToolTranslate::mouseUp(x,y);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}

void G_BaseToolCreate::cancel()
{
	G_Selection::getSingletonRef().clearSelection();

	G_EditorApp::getSingletonRef().getMap().remObject(_object);

	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);

	G_EditorApp::getSingletonRef().refresh();
}