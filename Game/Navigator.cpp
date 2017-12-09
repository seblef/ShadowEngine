
#include "stdafx.h"
#include "Navigator.h"

#define NAV_CONVERT_POS(x)			(int)x
#define NAV_ORIENTATION_EPSILON		2.0f
#define NAV_MAX_ORIENTATION_DIFF	45.0f

extern ofstream				g_MediaOut;

Navigator::Navigator() :
	_controller(0), _forwardSpeed(0), _backwardSpeed(0), _rotationSpeed(0),
	_currentPath(0), _currentPathStep(0), _pos(Vector2::NullVector), _posX(0), _posY(0), _orientation(0),
	_desireOrientation(false), _desiredOrientation(0), _desireGoal(false), _desiredGoal(Vector2::NullVector)
{
}

Navigator::~Navigator()
{
	if (_currentPath)
		_currentPath->Delete();
}

void Navigator::setPosition(const Vector2& pos)
{
	_pos = pos;
	_posX = NAV_CONVERT_POS(pos.x);
	_posY = NAV_CONVERT_POS(pos.y);
}

void Navigator::setPosition(const Vector3& pos)
{
	_pos.x = pos.x;
	_pos.y = pos.z;
	_posX = NAV_CONVERT_POS(pos.x);
	_posY = NAV_CONVERT_POS(pos.z);
}

void Navigator::setOrientation(float orientation)
{
	_orientation = orientation;
}

bool Navigator::enableGoal(const Vector2& goal, bool overrideGoal)
{
	int dx, dy;

	dx = NAV_CONVERT_POS(goal.x);
	dy = NAV_CONVERT_POS(goal.y);

	if (_posX == dx && _posY == dy)
	{
		disableGoal();
		return true;
	}

	if (_currentPath)
		_currentPath->clearAllocated();
	else
		_currentPath = NavPath::New();

	if (Navigation::getSingletonRef().findPath(_posX, _posY, dx, dy, *_currentPath))
	{
		_currentPathStep = _currentPath->getPathLength() - 1;
		_desiredGoal = goal;
		_desireGoal = true;

		NavRenderer::getSingletonRef().setNavPath(_currentPath);

		return true;
	}
	else if (overrideGoal)
	{
		_currentPathStep = 0;
		_desireGoal = false;
		return false;
	}
	else
		return _currentPathStep > 0;
}

void Navigator::disableGoal()
{
	_desireGoal = false;
	_currentPathStep = 0;

	NavRenderer::getSingletonRef().setNavPath(0);

	if (_currentPath)
	{
		_currentPath->Delete();
		_currentPath = 0;
	}
}

bool Navigator::enableOrientation(float desiredOrientation)
{
	if (FloatMakePositive(desiredOrientation - _orientation) > NAV_ORIENTATION_EPSILON)
	{
		_desireOrientation = true;
		_desiredOrientation = desiredOrientation;
		return true;
	}
	else
	{
		_desireOrientation = false;
		return false;
	}
}

void Navigator::disableOrientation()
{
	_desireOrientation = false;
}

bool Navigator::update(float time)
{
	if (_desireGoal)
	{
		const NavCase& c(_currentPath->getPathStep(_currentPathStep - 1));
		Vector2 subGoal(0.5f + (float)c.getX(), 0.5f + (float)c.getY());
		Vector2 d(subGoal - _pos);
		float lg = d.getLength();
		d /= lg;

		float rot = acosf(d.y) * 180.0f / M_PI;
		if (d.x < 0)
			rot = -rot;

		updateOrientation(rot,time);

		lg = smin(lg, _forwardSpeed * time);

		Vector3 delta(0, 0, lg);

		Matrix4 mrot;
		mrot.createRotateY(_orientation*M_PI / 180.0f);
		delta *= mrot;
		delta.y -= 9.81f * time;//		gravity

		_pos3D = _controller->move(delta, time);
		setPosition(_pos3D);

		if (_posX == c.getX() && _posY == c.getY())
		{
			--_currentPathStep;
			if (_currentPathStep == 0)
			{
				disableGoal();
				return false;
			}
		}

		return true;
	}
	else if (_desireOrientation)
	{
		updateOrientation(_orientation, time);
		if (FloatMakePositive(_orientation - _desiredOrientation) < NAV_ORIENTATION_EPSILON)
		{
			disableOrientation();
			return false;
		}
		else
			return true;
	}
	else
		return false;
}

void Navigator::updateOrientation(float goalOrientation, float time)
{
	if (_desireOrientation)
	{
		float dg = alignAngleDifference(goalOrientation, _desiredOrientation);
		if (dg < -NAV_MAX_ORIENTATION_DIFF)
			dg = -NAV_MAX_ORIENTATION_DIFF;
		else if (dg > NAV_MAX_ORIENTATION_DIFF)
			dg = NAV_MAX_ORIENTATION_DIFF;

		goalOrientation += dg;
		alignAngle(goalOrientation);
	}
	float gd=alignAngleDifference(_orientation,goalOrientation);

	if (gd < 0.0f)
		_orientation -= smin(-gd, _rotationSpeed * time);
	else if (gd > 0.0f)
		_orientation += smin(gd, _rotationSpeed * time);

	alignAngle(_orientation);
}