
#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_

#include "GameCharacter.h"

class GamePlayer : public GameCharacter
{
protected:

	bool					_keys[256];

	unsigned int			_lastMovement;
	unsigned int			_movement;

	float					_desiredRotation;
	float					_desiredSpineRotation;
	bool					_wantFire;

	int						_spineBoneIdx;
	Matrix4					_spineBoneOffset;
	float					_spineBoneRotation;

public:

	GamePlayer(Character& p, const Vector2& pos);
	~GamePlayer()																{}

	void						setDesiredRotation(float r)						{ _desiredRotation=r; }
	void						setDesiredSpineRotation(float r)				{ _desiredSpineRotation = r; }
	void						setFire(bool fire)								{ _wantFire = fire; }

	void						onKeyDown(int k);
	void						onKeyUp(int k);

	void						update(float time);
	void						move(unsigned int movement)						{ _movement|=movement; }

	void						dies()											{}
};

#endif