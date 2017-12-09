
#include "stdafx.h"
#include "G_GroundToolSetStartPos.h"
#include "G_EditorApp.h"


void G_GroundToolSetStartPos::mouseMove(int x, int y)
{
	Vector3 p;

	if(getGroundPosition(x,y,p))
	{
		Vector2 sp;
		sp.x=p.x;
		sp.y=p.z;

		G_Map& m(G_EditorApp::getSingletonRef().getMap());

		sclamp(sp.x,1.0f,(float)(m.getGround().getWidth()-2));
		sclamp(sp.y,1.0f,(float)(m.getGround().getHeight()-2));

		m.setStartPosition(sp);
		G_EditorApp::getSingletonRef().refresh();
	}

}

void G_GroundToolSetStartPos::draw(G_EditorObjDrawer* d)
{
	const Vector2& sp(G_EditorApp::getSingletonRef().getMap().getStartPosition());
	Vector3 p(sp.x,1,sp.y);

	d->setColor(Color::Green);
	d->drawDot(p,2);
}