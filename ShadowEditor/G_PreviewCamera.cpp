
#include "stdafx.h"
#include "G_PreviewCamera.h"

void G_PreviewCamera::update()
{
	Quaternion rot_y(Vector3::YAxisVector,_rot.y*M_PI / 180.0f);
	Vector3 z(Vector3::ZAxisVector);
	Vector3 x(Vector3::XAxisVector);

	x=x*rot_y;
	z=z*rot_y;

	Quaternion rot_x(x,_rot.x*M_PI/180.0f);
	z=z*rot_x;

	_cam.setPosition(_cam.getTarget()-(z*_dist));
	_cam.buildMatrices();
}
