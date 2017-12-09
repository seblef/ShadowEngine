
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "BBox3.h"
#include "Vector2.h"

namespace Core
{

enum CameraType
{
	CT_PERSPECTIVE=0,
	CT_ORTHO,

	CT_COUNT
};

class Camera
{
protected:

	Vector3				_pos;
	Vector3				_target;
	Vector2				_center;
	Vector3				_up;

	float				_ZMin;
	float				_ZMax;
	float				_fov;

	Matrix4				_viewMatrix;
	Matrix4				_projMatrix;
	Matrix4				_viewProjMatrix;
	Matrix4				_invViewMatrix;

	Vector3				_XAxis;
	Vector3				_YAxis;

	CameraType			_type;
	float				_zoom;

public:

	Camera();
	Camera(const Vector3& pos, const Vector3& target, float zmin, float zmax, const Vector2& center, float fov);

	void				center(const BBox3& bBox);

	void				setPosition(const Vector3& vPos)				{ _pos=vPos; }
	void				setTarget(const Vector3& vTarget)				{ _target=vTarget; }
	void				setZMin(float fZMin)							{ _ZMin=fZMin; }
	void				setZMax(float fZMax)							{ _ZMax=fZMax; }
	void				setCenter(const Vector2& vCenter)				{ _center=vCenter; }
	void				setFOV(float fFOV)								{ _fov=fFOV; }
	void				setCameraType(CameraType type)					{ _type=type; }
	void				setZoom(float fZoom)							{ _zoom=fZoom; }
	void				setUpVector(const Vector3& vUp)					{ _up=vUp; }

	const Vector3&		getPosition() const								{ return _pos; }
	const Vector3&		getTarget() const								{ return _target; }
	float				getZMin() const									{ return _ZMin; }
	float				getZMax() const									{ return _ZMax; }
	const Vector2&		getCenter() const								{ return _center; }
	float				getFOV() const									{ return _fov; }
	CameraType			getCameraType() const							{ return _type; }
	float				getZoom() const									{ return _zoom; }
	const Vector3&		getUpVector() const								{ return _up; }

	void				buildMatrices();
	const Matrix4&		getViewMatrix() const							{ return _viewMatrix; }
	const Matrix4&		getProjectionMatrix() const						{ return _projMatrix; }
	const Matrix4&		getViewProjMatrix() const						{ return _viewProjMatrix; }

	const Vector3&		getXAxis() const								{ return _XAxis; }
	const Vector3&		getYAxis() const								{ return _YAxis; }

	void				makeRayFrom2D(int x, int y, Vector3& vOrigin, Vector3& vDir);
};

}

#endif