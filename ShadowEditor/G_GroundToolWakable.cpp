
#include "stdafx.h"
#include "G_GroundToolWakable.h"
#include "G_EditorApp.h"
#include "G_Camera.h"
#include "../Game/Ground.h"


void G_GroundToolWakable::mouseDown(int x, int y, unsigned short flags)
{
	if(_point)
		mouseMove(x,y);
	else
	{
		Vector3 p;
		if(getGroundPosition(x,y,p))
		{
			_lastX=_startX=(int)p.x;
			_lastY=_startY=(int)p.z;
		}
		else
			_lastX=_lastY=_startX=_startY=-1;
	}
}

void G_GroundToolWakable::mouseMove(int x, int y)
{
	Vector3 p;
	if(!getGroundPosition(x,y,p))
		return;

	int xp=(int)p.x;
	int yp=(int)p.z;

	if(_point)
	{
		if(_wakable)
			G_EditorApp::getSingletonRef().getMap().getGround().setFlag(xp,yp,GFLAG_WAKABLE);
		else
			G_EditorApp::getSingletonRef().getMap().getGround().unsetFlag(xp,yp,GFLAG_WAKABLE);
	}
	else
	{
		if(_startX==-1)
			mouseDown(x,y,0);
		else
		{
			_lastX=xp;
			_lastY=yp;
		}
	}

	G_EditorApp::getSingletonRef().refresh();
}

void G_GroundToolWakable::mouseUp(int mx, int my)
{
	if(!_point && _startX!=-1)
	{
		int sx,ex,sy,ey;
		sminmax(_startX,_lastX,sx,ex);
		sminmax(_startY,_lastY,sy,ey);

		sclamp(sx,0,G_EditorApp::getSingletonRef().getMap().getGround().getWidth()-1);
		sclamp(ex,0,G_EditorApp::getSingletonRef().getMap().getGround().getWidth()-1);

		sclamp(sy,0,G_EditorApp::getSingletonRef().getMap().getGround().getHeight()-1);
		sclamp(ey,0,G_EditorApp::getSingletonRef().getMap().getGround().getHeight()-1);

		if(_wakable)
		{
			for(int y=sy;y<=ey;++y)
				for(int x=sx;x<=ex;++x)
					G_EditorApp::getSingletonRef().getMap().getGround().setFlag(x,y,GFLAG_WAKABLE);
		}
		else
		{
			for(int y=sy;y<=ey;++y)
				for(int x=sx;x<=ex;++x)
					G_EditorApp::getSingletonRef().getMap().getGround().unsetFlag(x,y,GFLAG_WAKABLE);
		}
	}

	G_EditorApp::getSingletonRef().refresh();

	_startX=_startY=_lastX=_lastY=-1;
}

void G_GroundToolWakable::draw(G_EditorObjDrawer* d)
{
	if(_startX!=-1)
	{
		d->setColor(Color::White);
		Vector3 v[4];

		int sx,ex,sy,ey;
		sminmax(_startX,_lastX,sx,ex);
		sminmax(_startY,_lastY,sy,ey);

		sclamp(sx,0,G_EditorApp::getSingletonRef().getMap().getGround().getWidth()-1);
		sclamp(ex,0,G_EditorApp::getSingletonRef().getMap().getGround().getWidth()-1);

		sclamp(sy,0,G_EditorApp::getSingletonRef().getMap().getGround().getHeight()-1);
		sclamp(ey,0,G_EditorApp::getSingletonRef().getMap().getGround().getHeight()-1);

		for(int i=0;i<4;++i)
			v[i].y=0;

		v[0].x=(float)sx;
		v[0].z=(float)sy;

		v[1].x=(float)(ex+1);
		v[1].z=(float)sy;

		v[2].x=(float)(ex+1);
		v[2].z=(float)(ey+1);

		v[3].x=(float)sx;
		v[3].z=(float)(ey+1);

		d->drawRect(v[0],v[1],v[2],v[3]);
	}
}