
#ifndef _G_PREVIEWCAMERA_H_
#define _G_PREVIEWCAMERA_H_

#include <Core.h>

using namespace Core;

class G_PreviewCamera
{
protected:

	Camera					_cam;
	Vector2					_rot;
	float					_dist;

	void					update();

public:

	G_PreviewCamera(const Vector2& rot=Vector2::NullVector, float dist=10.0f) : _cam(-Vector3::ZAxisVector,Vector3::NullVector,0.1f,100.0f,Vector2(10,10),90),
		_rot(rot), _dist(dist)		{ update(); }

	void					rotate(const Vector2& r)			{ _rot+=r; update(); }
	void					center(const BBox3& b)
	{
		_cam.setTarget(b.getCenter());
		_dist=b.getSize().getLength() * 0.8f;
		update();
	}

	void				onResize(int w, int h)
	{
		_cam.setCenter(Vector2((float)(w/2), (float)(h/2)));
	}

	Camera&					getCamera()							{ return _cam; }

	const Vector2&			getRotation() const					{ return _rot; }
	float					getDistance() const					{ return _dist; }
};

#endif