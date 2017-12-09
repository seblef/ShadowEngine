
#include "stdafx.h"
#include "GameCamera.h"
#include <SoundLib.h>

GameCamera::GameCamera(GameCharacter* target, float dist, Vector2 rot, float screen_width, float screen_height) :
	_cam(Vector3::NullVector, Vector3::NullVector, 1.0f, 50.0f, Vector2(screen_width*0.5f, screen_height*0.5f), 90.0f),
	_target(target), _dist(dist), _rot(rot), _lastPos(Vector3::NullVector), _trackDirection(true)
{

}

void GameCamera::update(float elapsed)
{
	_lastPos = _cam.getPosition();

	Vector3 t(_target->getAgentPosition());
	_cam.setTarget(t);

	float ry = _rot.y + _target->getNavigator().getOrientation();

	Quaternion rot_y(Vector3::YAxisVector,ry*M_PI / 180.0f);
	Vector3 z(Vector3::ZAxisVector);
	Vector3 x(Vector3::XAxisVector);

	x=x*rot_y;
	z=z*rot_y;

	Quaternion rot_x(x,_rot.x*M_PI/180.0f);
	z=z*rot_x;

	_cam.setPosition(t-(z*_dist));

	//		Updating sound listener

	Vector3 vel(Vector3::NullVector);
//	if (elapsed != 0.0f)
//		vel = (_cam.getPosition() - _lastPos) / elapsed;

	ISoundListener* l= SoundSystem::getSingletonRef().getAudio()->getListener();
	l->setPosition(_cam.getPosition());
	l->setVelocity(vel);
	l->setTopAxis(x ^ z);
	l->setFrontAxis(z);
}