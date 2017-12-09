
#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

#include <NavigationLib.h>
#include <PhysicLib.h>

class Navigator
{
protected:

	PhysicActorController*	_controller;
	NavPath*				_currentPath;
	int						_currentPathStep;

	Vector2					_pos;
	Vector3					_pos3D;
	int						_posX, _posY;
	float					_orientation;

	bool					_desireOrientation;
	float					_desiredOrientation;

	bool					_desireGoal;
	Vector2					_desiredGoal;

	float					_forwardSpeed;
	float					_backwardSpeed;
	float					_rotationSpeed;

	void					updateOrientation(float goalOrientation, float time);
	float					alignAngleDifference(float from, float to) const
	{
		float d = to - from;
		if (d < -180.0f)			d = 360.0f + d;
		else if (d > 180.0f)		d = d - 360.0f;

		return d;
	}
	void					alignAngle(float& a) const
	{
		if (a < -180.0f)			a += 360.0f;
		else if (a > 180.0f)		a -= 360.0f;
	}

public:

	Navigator();
	~Navigator();

	void					setActorController(PhysicActorController* ctrl)		{ _controller = ctrl; }
	void					setForwardSpeed(float s)							{ _forwardSpeed = s; }
	void					setBackwardSpeed(float s)							{ _backwardSpeed = s; }
	void					setRotationSpeed(float s)							{ _rotationSpeed = s; }

	void					setPosition(const Vector2& pos);
	void					setPosition(const Vector3& pos);
	void					setPosition3D(const Vector3& pos3D)					{ _pos3D = pos3D; }
	void					setOrientation(float orientation);

	bool					enableGoal(const Vector2& goal, bool overrideGoal);
	void					disableGoal();

	bool					enableOrientation(float desiredOrientation);
	void					disableOrientation();

	const Vector2&			getPosition() const						{ return _pos; }
	const Vector3&			getPosition3D() const					{ return _pos3D; }
	void					getPosition(int& x, int& y) const		{ x = _posX; y = _posY; }
	int						getPosX() const							{ return _posX; }
	int						getPosY() const							{ return _posY; }

	float					getOrientation() const					{ return _orientation; }

	bool					isGoalEnable() const					{ return _desireGoal; }
	const Vector2&			getGoal() const							{ return _desiredGoal; }
	NavPath*				getCurrentPath() const					{ return _currentPath; }

	bool					isOrientationEnable() const				{ return _desireOrientation; }
	float					getDesiredOrientation() const			{ return _desiredOrientation; }

	bool					isMoving() const						{ return _currentPathStep > 0 || _desireOrientation; }
	bool					update(float time);
};

#endif