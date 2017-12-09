
#include "stdafx.h"
#include "G_BaseToolPaste.h"
#include "G_ToolSystem.h"
#include "G_Selection.h"
#include "G_EditorApp.h"

void G_BaseToolPaste::mouseDown(int x, int y, unsigned short flags)
{
	_flags=flags;

	Vector3 center(Vector3::NullVector);
	const G_Selection::SelectionSet& ss(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(ss.begin());
	float count=0;
	for(;o!=ss.end();++o,count+=1.0f)
	{
		center+=(*o)->getPosition();
		G_EditorApp::getSingletonRef().getMap().addObject(*o);
	}

	center/=count;
	
	Vector3 pos,p(Vector3::NullVector);
	getGroundPosition(x,y,p);

	_lastPos=p;
	_lastPos.y=0;

	p-=center;

	for(o=ss.begin();o!=ss.end();++o)
	{
		const Vector3& oldPos((*o)->getPosition());
		pos.x=oldPos.x + p.x;
		pos.y=oldPos.y;
		pos.z=oldPos.z + p.z;
		(*o)->setPosition(pos);

		_originals.push_back(pos);

		if(_flags & GTOOL_SNAPGRID)
			snap(*o);
	}

	_translation=Vector3::NullVector;

	G_EditorApp::getSingletonRef().refresh();
}

void G_BaseToolPaste::mouseUp(int x, int y)
{
	G_BaseToolTranslate::mouseUp(x,y);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}

void G_BaseToolPaste::cancel()
{
	const G_Selection::SelectionSet& ss(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(ss.begin());
	for(;o!=ss.end();++o)
		G_EditorApp::getSingletonRef().getMap().remObject(*o);

	G_Selection::getSingletonRef().clearSelection();

	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
	G_EditorApp::getSingletonRef().refresh();
}