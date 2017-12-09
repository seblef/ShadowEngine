
#include "stdafx.h"
#include "G_BaseToolRotateZ.h"
#include "G_Selection.h"
#include "G_EditorApp.h"


void G_BaseToolRotateZ::mouseMove(int x, int y)
{
	Vector3 r(0,0,(float)(y-_lastY));
	_rotation+=r;
	_lastX=x;
	_lastY=y;

	if(_flags & GTOOL_LOCKX)			r.x=0;
	if(_flags & GTOOL_LOCKY)			r.y=0;
	if(_flags & GTOOL_LOCKZ)			r.z=0;

	const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(sel.begin());
	Vec3Vector::const_iterator rot(_originals.begin());
	for(;o!=sel.end();++o,++rot)
	{
		(*o)->setRotation((*rot) + _rotation);

		if(_flags & GTOOL_SNAPGRID)
			snap(*o);
	}

	G_EditorApp::getSingletonRef().refresh();
}