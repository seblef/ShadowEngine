
#include "stdafx.h"
#include "GamePlayer.h"

//#define HDAO_TUNING

#ifdef HDAO_TUNING
#include "../Renderer/HDAO.h"
#endif

const float				ActorMaxSpineRotation = 5.0f;
const float				ActorMinSpineRotation = -20.0f;
const float				ActorSpineRotationSpeed = 120.0f;

extern ofstream			g_MediaOut;

GamePlayer::GamePlayer(Character& p, const Vector2& pos) : GameCharacter(p, pos, 0, false), _movement(0),
_lastMovement(0), _desiredRotation(0), _wantFire(false), _spineBoneIdx(-1),
_spineBoneOffset(Matrix4::Identity), _spineBoneRotation(0.0f), _desiredSpineRotation(0)
{
	for (int i = 0; i<256; ++i)
		_keys[i] = false;

	_spineBoneIdx = _inst->getModel()->getBoneIndex("Spine1");
	if (_spineBoneIdx != -1)
		_inst->getWorldMatrices().enableOffset(_spineBoneIdx);
}

void GamePlayer::onKeyDown(int k)
{
	_keys[k] = true;

#ifdef HDAO_TUNING
	HDAO& h(HDAO::getSingletonRef());

	if (k == DIK_F1)
		h.getAcceptRadius() += 0.001f;
	else if (k == DIK_F2)
		h.getAcceptRadius() -= 0.001f;
	else if (k == DIK_F3)
		h.getRejectRadius() += 0.01f;
	else if (k == DIK_F4)
		h.getRejectRadius() -= 0.01f;
	else if (k == DIK_F5)
		h.getAcceptAngle() += 0.01f;
	else if (k == DIK_F6)
		h.getAcceptAngle() -= 0.01f;
	else if (k == DIK_F7)
		h.getNormalScale() += 0.01f;
	else if (k == DIK_F8)
		h.getNormalScale() -= 0.01f;
	else if (k == DIK_F9)
		h.getIntensity() += 0.04f;
	else if (k == DIK_F10)
		h.getIntensity() -= 0.04f;
#endif
}

void GamePlayer::onKeyUp(int k)
{
	_keys[k] = false;
}

void GamePlayer::update(float time)
{
	if (_wantFire)
		fire();

	unsigned int m=0;
	if(_keys[DIK_UP])			m=CM_FORWARD;
	if(_keys[DIK_DOWN])			m|=CM_BACKWARD;
	if(_keys[DIK_E])			m|=CM_LEFT;
	if(_keys[DIK_R])			m|=CM_RIGHT;

	move(m);

	Vector3 delta(Vector3::NullVector);

	if(_lastMovement && _movement==0)
		_inst->playAnimation(_T("pose"));

	if(_movement & CM_FORWARD)
	{
		if((_lastMovement & CM_FORWARD)==0)
			_inst->playAnimation(_T("walk"));

		delta.z+=_template->getForwardSpeed();
	}
	else if(_movement & CM_BACKWARD)
	{
		if((_lastMovement & CM_BACKWARD)==0)
			_inst->playAnimation(_T("walk"),true,true);
		delta.z-=_template->getBackwardSpeed();
	}

	if(_movement & CM_RIGHT)
		delta.x-=_template->getLateralSpeed();
	else if(_movement & CM_LEFT)
		delta.x+=_template->getLateralSpeed();
	
	float maxRot = _template->getRotationSpeed() * time;
	sclamp(_desiredRotation, -maxRot, maxRot);

	_navigator.setOrientation(_navigator.getOrientation() + _desiredRotation);
	_desiredRotation = 0.0f;

	if (_spineBoneIdx != -1)
	{
		float maxSpineRot = ActorSpineRotationSpeed * time;
		sclamp(_desiredSpineRotation, -maxSpineRot, maxSpineRot);
		_spineBoneRotation += _desiredSpineRotation;
		sclamp(_spineBoneRotation, ActorMinSpineRotation, ActorMaxSpineRotation);
		_desiredSpineRotation = 0.0f;

		_spineBoneOffset.createRotateY(_spineBoneRotation * M_PI / 180.0f);
		_inst->getWorldMatrices().setOffset(_spineBoneIdx, _spineBoneOffset);
	}

	Matrix4 mrot;
	mrot.createRotateY(_navigator.getOrientation()*M_PI/180.0f);
	delta*=mrot;
	delta.y -= 9.81f;//		gravity
	delta*=time;

	int oldx, oldy;
	_navigator.getPosition(oldx, oldy);

	_agentPos=_controller->move(delta,time);
	_navigator.setPosition(_agentPos);

	int newx, newy;
	_navigator.getPosition(newx, newy);
 
	if (oldx != newx || oldy != newy)
		onMove(oldx, oldy, newx, newy);

	makeWorldMatrix();
	_inst->setWorldMatrix(_world);

	_lastMovement=_movement;
	_movement=0;
}