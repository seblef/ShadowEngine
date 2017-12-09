
#ifndef _SYSTEMVALUES_H_
#define _SYSTEMVALUES_H_

#include <Core.h>

using namespace Core;

class SystemValues : public TSingleton<SystemValues>
{
protected:

	bool				_demoMode;
	bool				_showFPS;
	string				_hudFont;
	string				_crossTexture;
	float				_crossSize;

	float				_cameraBaseDistance;

	Vector3				_gravity;
	float				_damageToForce;

	int					_baseDebrisCount;
	float				_baseDebrisMinVelocity;
	float				_baseDebrisMaxVelocity;
	float				_baseDebrisAngle;
	float				_baseDebrisAngleSpeed;
	float				_baseDebrisSize;

	float				_aiDamageFactor;
	float				_playerDamageFactor;
	float				_deadActorStayOnMapTime;

public:

	SystemValues() : _demoMode(true),
		_showFPS(true),
		_hudFont("2D/Font.sfn"),
		_crossTexture("Textures/HUD/cross.dds"),
		_crossSize(10.0f),
		_cameraBaseDistance(5.0f),
		_gravity(0.0f,-9.81f,0.0f),
		_baseDebrisCount(5),
		_baseDebrisMinVelocity(1.0f),
		_baseDebrisMaxVelocity(10.0f),
		_baseDebrisAngle(0.25f * M_PI),
		_baseDebrisAngleSpeed(M_PI),
		_baseDebrisSize(0.01f),
		_aiDamageFactor(1.2f),
		_playerDamageFactor(0.8f),
		_deadActorStayOnMapTime(1.0f),
		_damageToForce(1.0f)
		{}
	~SystemValues()		{}

	void				parseConfig(Config& cfg);

	bool				getDemoMode() const						{ return _demoMode; }
	bool				getShowFPS() const						{ return _showFPS; }
	const string&		getHUDFont() const						{ return _hudFont; }
	const string&		getCrossTexture() const					{ return _crossTexture; }
	float				getCrossSize() const					{ return _crossSize; }

	float				getCameraBaseDistance() const			{ return _cameraBaseDistance; }

	const Vector3&		getGravity() const						{ return _gravity; }
	float				getDamageToForce() const				{ return _damageToForce; }

	int					getBaseDebrisCount() const				{ return _baseDebrisCount; }
	float				getBaseDebrisMinVelocity() const		{ return _baseDebrisMinVelocity; }
	float				getBaseDebrisMaxVelocity() const		{ return _baseDebrisMaxVelocity; }
	float				getBaseDebrisAngle() const				{ return _baseDebrisAngle; }
	float				getBaseDebrisAngleSpeed() const			{ return _baseDebrisAngleSpeed; }
	float				getBaseDebrisSize() const				{ return _baseDebrisSize; }

	float				getAIDamageFactor() const				{ return _aiDamageFactor; }
	float				getPlayerDamageFactor() const			{ return _playerDamageFactor; }
	float				getDeadActorStayOnMapTime() const		{ return _deadActorStayOnMapTime; }
};

#endif