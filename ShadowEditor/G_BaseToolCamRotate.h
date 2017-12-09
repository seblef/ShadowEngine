
#ifndef _G_BASETOOLCAMROTATE_H_
#define _G_BASETOOLCAMROTATE_H_

#include "G_Tool.h"
#include "G_Camera.h"
#include "G_EditorApp.h"

using namespace Core;

class G_BaseToolCamRotate : public G_Tool
{
protected:

	int				_lastX,_lastY;
	Vector2			_rot;

public:

	G_BaseToolCamRotate()			{}

	void			mouseDown(int x, int y, unsigned short flags)
	{
		_rot=Vector2::NullVector;
		_lastX=x;
		_lastY=y;
	}

	void			mouseMove(int x, int y)
	{
		Vector2 rot((float)(y-_lastY),(float)(x-_lastX));
		_rot+=rot;
		G_Camera::getSingletonRef().rotate(rot);
		_lastX=x;
		_lastY=y;
		G_EditorApp::getSingletonRef().refresh();
	}

	void			cancel()
	{
		G_Camera::getSingletonRef().rotate(-_rot);
		G_EditorApp::getSingletonRef().refresh();
	}
};

#endif