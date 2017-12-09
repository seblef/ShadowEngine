
#include "stdafx.h"
#include "G_Object.h"

void G_Object::rebuildMatrix()
{
	Matrix4 rot_x,rot_y,rot_z;
	rot_x.createRotateX(_rot.x * M_PI / 180.0f);
	rot_y.createRotateY(_rot.y * M_PI / 180.0f);
	rot_z.createRotateZ(_rot.z * M_PI / 180.0f);

	_world=rot_z*rot_y;
	_world*=rot_x;
	_world=_pos;
	if(_selItem)
		_selItem->updateWorldMatrix(_world);
}