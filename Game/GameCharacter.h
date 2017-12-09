
#ifndef _GAMECHARACTER_H_
#define _GAMECHARACTER_H_

#include "Character.h"
#include "GameMesh.h"
#include "WeaponInstance.h"
#include "Navigator.h"
#include "Hit.h"
#include <PhysicLib.h>
#include <AILib.h>

class HUDActor;

#define CM_BACKWARD					(1 << 0)
#define CM_FORWARD					(1 << 1)
#define CM_LEFT						(1 << 2)
#define CM_RIGHT					(1 << 3)
#define CM_TURNRIGHT				(1 << 4)
#define CM_TURNLEFT					(1 << 5)

enum GameCharacterState
{
	GCS_STAND=0,
	GCS_WALKING,
	GCS_RUNNING,
	GCS_ORIENTATING,

	GCS_DEAD,

	GCS_COUNT
};

class GameCharacter : public GameObject, public IAICallback
{
protected:

	Character*				_template;
	ActorInstance*			_inst;
	PhysicActorController*	_controller;
	HUDActor*				_hud;
	unsigned int			_aiAgent;
	Navigator				_navigator;
	Vector2					_home;
	bool					_isPlayer;
	float					_life;
	bool					_dead;

	float					_updateTime;

	GameCharacterState		_currentState;

	WeaponInstance*			_weapon;

	Matrix4					_world;
	BBox3					_box;

	void					makeWorldMatrix();
	void					updateSensors();
	bool					goTo(const Vector3& pos, const Vector3& dir, GameCharacterState state);

public:

	GameCharacter(Character& p, const Vector2& pos, float orientation, bool ai);
	~GameCharacter();

	void				onAddToScene();
	void				onRemFromScene();
	void				onMove(int oldx, int oldy, int newx, int newy);

	virtual void		update(float time);

	NavPath*			getPath() const							{ return _navigator.getCurrentPath(); }
	const Navigator&	getNavigator() const					{ return _navigator; }

	ActorInstance*		getActorInstance() const				{ return _inst; }
	IPhysicObject*		getPhysicObject() const					{ return _controller; }

	void				setWeapon(Weapon *w);

	void				changeState(GameCharacterState s);

	unsigned int		getAgentID() const						{ return _aiAgent; }
	bool				isPlayer() const						{ return _isPlayer; }

	Character*			getTemplate() const						{ return _template; }
	WeaponInstance*		getWeapon() const						{ return _weapon; }

	void				onHit(const Hit& hit);
	virtual void		dies();

	float				getLife() const							{ return _life; }
	bool				isDead() const							{ return _dead; }

	//			IA Callback

	void				idle();
	bool				walkTo(const Vector3& pos, const Vector3& dir);
	bool				runTo(const Vector3& pos, const Vector3& dir);
	void				orientTo(const Vector3& dir);
	void				goHome();
	void				fire();
	bool				canFire()
	{
		return _weapon->canFire();
	}
	float				getFireRange()
	{
		return _weapon->getTemplate()->getRange();
	}
};

#endif