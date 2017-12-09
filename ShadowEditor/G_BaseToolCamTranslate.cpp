
#include "stdafx.h"
#include "G_BaseToolCamTranslate.h"


void G_BaseToolCamTranslate::mouseDown(int x, int y, unsigned short flags)
{
	Vector3 p;
	if(getGroundPosition(x,y,p))
	{
		G_Camera::getSingletonRef().setTargetPosition(Vector2(p.x,p.z));
		G_EditorApp::getSingletonRef().refresh();
	}
}