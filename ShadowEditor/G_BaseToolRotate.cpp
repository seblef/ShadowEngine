
#include "stdafx.h"
#include "G_BaseToolRotate.h"
#include "G_Selection.h"
#include "G_EditorApp.h"
#include "G_Camera.h"


void G_BaseToolRotate::mouseDown(int x, int y, unsigned short flags)
{
	G_BaseToolNull::mouseDown(x,y,flags);

	_flags=flags;
	const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(sel.begin());
	for(;o!=sel.end();++o)
		_originals.push_back((*o)->getRotation());

	_lastX=x;
	_lastY=y;
	_rotation=Vector3::NullVector;
}

void G_BaseToolRotate::mouseMove(int x, int y)
{
	//		[TODO]: Snap on grid and x/y/z lock

	Vector3 r((float)(y-_lastY),(float)(x-_lastX),0);
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

void G_BaseToolRotate::cancel()
{
	const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(sel.begin());
	Vec3Vector::const_iterator v(_originals.begin());
	for(;o!=sel.end();++o,++v)
		(*o)->setRotation(*v);

	G_EditorApp::getSingletonRef().refresh();
}

void G_BaseToolRotate::snap(G_Object* o) const
{
	int rx=(int)o->getRotation().x;
	int ry=(int)o->getRotation().y;
	int rz=(int)o->getRotation().z;

	rx=(rx / 45) * 45;
	ry=(ry / 45) * 45;
	rz=(rz / 45) * 45;

	Vector3 r((float)rx,(float)ry,(float)rz);
	o->setRotation(r);
}