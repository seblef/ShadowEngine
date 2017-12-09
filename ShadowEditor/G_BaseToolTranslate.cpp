
#include "stdafx.h"
#include "G_BaseToolTranslate.h"
#include "G_Selection.h"
#include "G_EditorApp.h"
#include "G_Camera.h"


void G_BaseToolTranslate::mouseDown(int x, int y, unsigned short flags)
{
	G_BaseToolNull::mouseDown(x,y,flags);

	_flags=flags;
	const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(sel.begin());
	for(;o!=sel.end();++o)
		_originals.push_back((*o)->getPosition());

	if(!getGroundPosition(x,y,_lastPos))
		_lastPos=Vector3::NullVector;

	_translation=Vector3::NullVector;
}

void G_BaseToolTranslate::mouseMove(int x, int y)
{
	Vector3 p;
	if(getGroundPosition(x,y,p))
	{
		Vector3 d(p-_lastPos);

		if(_flags & GTOOL_LOCKX)			d.x=0;
		if(_flags & GTOOL_LOCKY)			d.y=0;
		if(_flags & GTOOL_LOCKZ)			d.z=0;

		_translation+=d;

		const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
		G_Selection::SelectionSet::const_iterator o(sel.begin());
		int i=0;
		for(;o!=sel.end();++o,++i)
		{
			(*o)->setPosition(_originals[i] + _translation);
			if(_flags & GTOOL_SNAPGRID)
				snap(*o);
		}

		_lastPos=p;

		G_EditorApp::getSingletonRef().refresh();
	}
}

void G_BaseToolTranslate::cancel()
{
	const G_Selection::SelectionSet& sel(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(sel.begin());
	Vec3Vector::const_iterator v(_originals.begin());
	for(;o!=sel.end();++o,++v)
		(*o)->setPosition(*v);

	G_EditorApp::getSingletonRef().refresh();
}

void G_BaseToolTranslate::snap(G_Object* o) const
{
	Vector3 p[8];
	float xmin=1e23f, zmin=1e23f;
	const Matrix4& w(o->getWorldMatrix());
	o->getLocalBox().getPoints(p);

	for(int i=0;i<8;++i)
	{
		p[i]*=w;
		xmin=smin(p[i].x,xmin);
		zmin=smin(p[i].z,zmin);
	}

	Vector3 d(floorf(xmin)-xmin,0,floorf(zmin) - zmin);
	o->translate(d);
}