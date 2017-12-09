
#ifndef _G_GROUNDTOOLSETSTARTPOS_H_
#define _G_GROUNDTOOLSETSTARTPOS_H_

#include "G_Tool.h"
#include <StdRenderer.h>

class G_GroundToolSetStartPos : public G_Tool
{
protected:

	Plane						_ground;
	unsigned short				_flags;

public:

	G_GroundToolSetStartPos() : _ground(Vector3::NullVector, Vector3::YAxisVector)				{}
	~G_GroundToolSetStartPos()						{}

	void						mouseDown(int x, int y, unsigned short flags)
	{
		_flags=flags;
		mouseMove(x,y);
	}
	void						mouseMove(int x, int y);

	void						draw(G_EditorObjDrawer* d);
};

#endif