
#ifndef _G_GROUNDTOOLWAKABLE_H_
#define _G_GROUNDTOOLWAKABLE_H_

#include "G_Tool.h"
#include <StdRenderer.h>

class G_GroundToolWakable : public G_Tool
{
protected:

	bool						_point;
	bool						_wakable;
	int							_startX,_startY;
	int							_lastX,_lastY;
	Plane						_ground;

public:

	G_GroundToolWakable() : _ground(Vector3::NullVector, Vector3::YAxisVector), _point(true), _wakable(true)		{}
	~G_GroundToolWakable()						{}

	void						setWakable(bool wakable)									{ _wakable=wakable; }
	void						setPoint(bool p)											{ _point=p; }

	void						mouseDown(int x, int y, unsigned short flags);
	void						mouseMove(int x, int y);
	void						mouseUp(int x, int y);

	void						draw(G_EditorObjDrawer* d);
};

#endif