
#include "stdafx.h"
#include "Character.h"
#include "ActorLoader.h"
#include "ActorAnimLoader.h"

Character::Character(ScriptFile& fl) : _forwardSpeed(1), _backwardSpeed(0.8f), _lateralSpeed(0.5f), _rotationSpeed(60.0f),
	_actor(_T("Test")), _rActor(0), _rMat(0), _aiMachine("Default"), _fov(M_PI / 2.0f), _visDistance(30.0f),
	_lifePoints(100.0f)
{
	_name=fl.getToken();

	string t(fl.getToken());
	while(fl.good() && t!=_T("end_character"))
	{
		if(t==_T("actor"))					_actor=fl.getToken();
		else if(t==_T("material"))			_material=fl.getToken();
		else if(t==_T("forward_speed"))		_forwardSpeed=stof(fl.getToken());
		else if(t==_T("backward_speed"))	_backwardSpeed=stof(fl.getToken());
		else if(t==_T("lateral_speed"))		_lateralSpeed=stof(fl.getToken());
		else if(t==_T("rotation_speed"))	_rotationSpeed=stof(fl.getToken());
		else if(t==_T("weapon"))			_weapon=fl.getToken();
		else if (t == "ai_machine")			_aiMachine = fl.getToken();
		else if (t == "ai_state")			_aiState = fl.getToken();
		else if (t == "fov")				_fov = stof(fl.getToken()) * M_PI / 180.0f;
		else if (t == "visibility_dist")	_visDistance = stof(fl.getToken());
		else if (t == "life")				_lifePoints = stof(fl.getToken());
		else if(t==_T("animation"))
		{
			_animNames.push_back(fl.getToken());
			_animFiles.push_back(fl.getToken());
		}

		t=fl.getToken();
	}
}

void Character::load()
{
	if(!_rMat && !_rActor)
	{
		_rMat=MaterialSystem::getSingletonRef().loadMaterial(_material);
		_rActor=ActorLoader::loadActor(_actor,_rMat,Renderer::getSingletonRef().getVideoDevice());

		for(int i=0;i<_animFiles.size();++i)
			_rActor->addAnimation(_animNames[i],ActorAnimLoader::loadActorAnimation(_animFiles[i]));
	}
}

void Character::unload()
{
	if(_rActor)			delete _rActor;
	if(_rMat)			delete _rMat;

	_rActor=0;
	_rMat=0;
}