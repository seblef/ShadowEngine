
#ifndef _G_CAMERA_H_
#define _G_CAMERA_H_

#include "G_PreviewCamera.h"

class G_Camera : public G_PreviewCamera, public TSingleton<G_Camera>
{
public:

	G_Camera() : G_PreviewCamera(Vector2(45.0f,45.0f),10.0f)			{}

	void				setTargetPosition(const Vector2& v)
	{
		_cam.setTarget(Vector3(v.x,0,v.y));
		update();
	}
	void				initialize(int ground_w, int ground_h)
	{
		setTargetPosition(Vector2((float)(ground_w/2),(float)(ground_h/2)));
	}
	void				translate(const Vector2& v)
	{
		setTargetPosition(Vector2(_cam.getTarget().x+v.x,_cam.getTarget().x+v.y));
	}

	void				scaleDist(float f)								{ _dist*=f; update(); }
};

#endif