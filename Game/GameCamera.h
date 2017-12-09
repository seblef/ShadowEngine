
#ifndef _GAMECAMERA_H_
#define _GAMECAMERA_H_

#include "GameCharacter.h"

class GameCamera
{
protected:

	Camera					_cam;
	GameCharacter*			_target;
	float					_dist;
	Vector2					_rot;
	Vector3					_lastPos;
	bool					_trackDirection;

public:

	GameCamera(GameCharacter* target, float dist, Vector2 rot, float screen_width, float screen_height);

	Camera&					getCamera()									{ return _cam; }
	GameCharacter*			getTarget() const							{ return _target; }
	float					getDistance() const							{ return _dist; }
	const Vector2&			getRotation() const							{ return _rot; }

	void					setTarget(GameCharacter* c, bool trackDir)	{ _target = c; _trackDirection = trackDir; }
	void					setDistance(float d)						{ _dist=d; }
	void					setRotation(const Vector2& d)				{ _rot=d; }

	void					update(float elapsed);
};

#endif