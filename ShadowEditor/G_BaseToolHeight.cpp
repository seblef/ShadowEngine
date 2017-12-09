
#include "stdafx.h"
#include "G_BaseToolHeight.h"
#include "G_Selection.h"
#include "G_EditorApp.h"
#include "G_Camera.h"


void G_BaseToolHeight::mouseDown(int x, int y, unsigned short flags)
{
	G_BaseToolNull::mouseDown(x,y,flags);

	Vector3 normal(G_Camera::getSingletonRef().getCamera().getTarget() - G_Camera::getSingletonRef().getCamera().getPosition());
	normal.normalize();

	const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(sel.begin());
	for(;o!=sel.end();++o)
	{
		_originals.push_back((*o)->getPosition().y);
		_plane.makePlane((*o)->getPosition(),normal);
	}

	Vector3 or,d,p;
	G_Camera::getSingletonRef().getCamera().makeRayFrom2D(x,y,or,d);

	if(_plane.intersectLine(or,d,p))
		_lastHeight=p.y;
	else
		_lastHeight=0;

	_translation=0;
}

void G_BaseToolHeight::mouseMove(int x, int y)
{
	Vector3 o,d,p;
	G_Camera::getSingletonRef().getCamera().makeRayFrom2D(x,y,o,d);

	if(_plane.intersectLine(o,d,p))
	{
		_translation+=(p.y - _lastHeight);
		_lastHeight=p.y;

		const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
		G_Selection::SelectionSet::const_iterator o(sel.begin());
		int i=0;
		Vector3 pos;
		for(;o!=sel.end();++o,++i)
		{
			pos=(*o)->getPosition();
			pos.y=_originals[i] + _translation;
			(*o)->setPosition(pos);
		}

		G_EditorApp::getSingletonRef().refresh();
	}
}

void G_BaseToolHeight::cancel()
{
	Vector3 pos;
	const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(sel.begin());
	FloatVector::const_iterator y(_originals.begin());

	for(;o!=sel.end();++o,++y)
	{
		pos=(*o)->getPosition();
		pos.y=*y;
		(*o)->setPosition(pos);
	}

	G_EditorApp::getSingletonRef().refresh();
}
