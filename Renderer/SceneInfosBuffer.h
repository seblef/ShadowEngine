
#ifndef _SCENEINFOSBUFFER_H_
#define _SCENEINFOSBUFFER_H_

#include <Core.h>

class SceneInfosBuffer
{
protected:

	Matrix4				_viewProj;
	Matrix4				_invViewProj;
	Matrix4				_invProj;
	Matrix4				_proj;
	Matrix4				_view;
	Vector4				_eyePos;
	Vector2				_invScreenSize;
	float				_time;
	float				_pack;

public:

	SceneInfosBuffer() : _viewProj(Matrix4::Identity), _invViewProj(Matrix4::Identity), _invProj(Matrix4::Identity),
		_proj(Matrix4::Identity), _view(Matrix4::Identity), _eyePos(Vector4::NullVector),
		_invScreenSize(Vector2::UnitVector), _time(0), _pack(0)																	{}

	void				setCamera(const Camera& c)
	{
		_viewProj=c.getViewProjMatrix();
		_proj=c.getProjectionMatrix();
		_viewProj.inverse(_invViewProj);
		_view = c.getViewMatrix();
		_proj.inverse(_invProj);
		_eyePos=c.getPosition();
		_invScreenSize=Vector2(0.5f / c.getCenter().x,0.5f / c.getCenter().y);
	}

	void				setTime(float time)						{ _time=time; }
};

#endif