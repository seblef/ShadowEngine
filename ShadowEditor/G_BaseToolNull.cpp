
#include "stdafx.h"
#include "G_BaseToolNull.h"
#include "G_Camera.h"
#include "G_Selection.h"
#include "G_EditorApp.h"

void G_BaseToolNull::mouseDown(int x, int y, unsigned short flags)
{
	Vector3 o,d;
	G_Camera::getSingletonRef().getCamera().makeRayFrom2D(x,y,o,d);

	bool g;
	G_Selection::getSingletonRef().select(o,d,flags & GTOOL_CTRLDOWN ? true : false,g);

	G_EditorApp::getSingletonRef().refresh();
}