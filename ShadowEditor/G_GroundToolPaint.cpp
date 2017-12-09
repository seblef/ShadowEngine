
#include "stdafx.h"
#include "G_GroundToolPaint.h"
#include "G_EditorApp.h"
#include "G_Camera.h"


void G_GroundToolPaint::mouseDown(int x, int y, unsigned short flags)
{
	G_EditorApp::getSingletonRef().getMap().getGround().beginPaint(_mat,_gameMat);
	mouseMove(x,y);
}

void G_GroundToolPaint::mouseMove(int x, int y)
{
	Vector3 p;
	if(getGroundPosition(x,y,p))
	{
		G_EditorApp::getSingletonRef().getMap().getGround().paint((int)p.x,(int)p.z);
		G_EditorApp::getSingletonRef().refresh();
	}
}

void G_GroundToolPaint::mouseUp(int x, int y)
{
	G_EditorApp::getSingletonRef().getMap().getGround().endPaint();
}