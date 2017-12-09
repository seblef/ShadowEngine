
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <StdRenderer.h>

class Character
{
protected:

	string				_name;
	string				_actor;
	string				_material;
	string				_weapon;
	string				_aiMachine;
	string				_aiState;
	float				_fov;
	float				_visDistance;
	float				_forwardSpeed;
	float				_backwardSpeed;
	float				_lateralSpeed;
	float				_rotationSpeed;
	float				_lifePoints;
	StringVector		_animNames;
	StringVector		_animFiles;

	Material*			_rMat;
	Actor*				_rActor;

public:

	Character(ScriptFile& fl);
	~Character()							{ unload(); }

	const string&							getName() const							{ return _name; }
	const string&							getWeapon() const						{ return _weapon; }
	const string&							getAIMachine() const					{ return _aiMachine; }
	const string&							getAIState() const						{ return _aiState; }
	float									getForwardSpeed() const					{ return _forwardSpeed; }
	float									getBackwardSpeed() const				{ return _backwardSpeed; }
	float									getLateralSpeed() const					{ return _lateralSpeed; }
	float									getRotationSpeed() const				{ return _rotationSpeed; }
	float									getFOV() const							{ return _fov; }
	float									getVisibilityDistance() const			{ return _visDistance; }
	float									getLifePoints() const					{ return _lifePoints; }

	Material*								getMaterial() const						{ return _rMat; }
	Actor*									getActor() const						{ return _rActor; }

	void									load();
	void									unload();
};

#endif