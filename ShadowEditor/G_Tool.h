
#ifndef _G_TOOL_H_
#define _G_TOOL_H_

#include "G_Camera.h"
#include "G_EditorObjDrawer.h"

#define GTOOL_LOCKX					(1 << 0)
#define GTOOL_LOCKY					(1 << 1)
#define GTOOL_LOCKZ					(1 << 2)
#define GTOOL_CTRLDOWN				(1 << 3)
#define GTOOL_SNAPGRID				(1 << 4)


class G_Tool
{
protected:

	Plane					_ground;

	bool					getGroundPosition(int x, int y, Vector3& p) const
	{
		Vector3 o,d;
		G_Camera::getSingletonRef().getCamera().makeRayFrom2D(x,y,o,d);

		float t;
		if(!_ground.intersectLine(o,d,t))
			return false;
		else if(t > 0)
		{
			p=o+d*t;
			return true;
		}
		else
			return false;
	}

public:

	G_Tool() : _ground(Vector3::NullVector,Vector3::YAxisVector)			{}
	virtual ~G_Tool()		{}

	virtual void			begin()											{}
	virtual void			end()											{}

	virtual void			mouseDown(int x, int y, unsigned short flags)	{}
	virtual void			mouseUp(int x, int y)							{}
	virtual void			mouseMove(int x, int y)							{}

	virtual void			cancel()										{}
	virtual void			draw(G_EditorObjDrawer* d)						{}
};

#endif