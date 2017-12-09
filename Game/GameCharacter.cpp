
#include "stdafx.h"
#include "GameSystem.h"
#include "GamePlayer.h"
#include "GameActorUpdater.h"
#include "GeometryLoader.h"
#include "BBoxToRect.h"
#include "SystemValues.h"
#include "ActionServer.h"
#include "AddRemObject.h"
#include "ActionCreateRandomCharacter.h"
#include "HUD.h"
#include "HUDActor.h"
#include <AILib.h>

#define AI_REFRESH_TIME			0.1f


extern ofstream		g_MediaOut;

//		[TODO]: Add animations

const char * const g_GameCharacterStateAnims[GCS_COUNT]={
	"pose",
	"walk",
	"walk",
	"walk",
	"pose"
};

GameCharacter::GameCharacter(Character& p, const Vector2& pos, float orientation, bool ai) :
	GameObject(GameObject::ACTOR,Matrix4::Identity,BBox3::NullBox,0), _template(&p),
	_weapon(0),	_currentState(GCS_COUNT), _aiAgent(0xffffffff), _updateTime(0),
	_home(pos), _isPlayer(!ai), _dead(false)
{
	p.load();
	_agentPos.x = pos.x;
	_agentPos.y=-p.getActor()->getBBox().getMin().y;
	_agentPos.z = pos.y;

	_agentDir.x = sinf(orientation*M_PI / 180.0f);
	_agentDir.y = 0;
	_agentDir.z = cosf(orientation * M_PI / 180.0f);

	_life = _template->getLifePoints();

	_hud = HUD::getSingletonRef().createHUDActor(_life, p.getActor()->getBBox().getMax().y);
	HUD::getSingletonRef().registerActor(_hud);

	makeWorldMatrix();
	_inst=new ActorInstance(p.getActor(),_world,true);
	Renderer::getSingletonRef().addRenderable(_inst);
	_inst->wakeUp();

	changeState(GCS_STAND);

	PhysicActorCtrlCreate_t c;
	c._box=p.getActor()->getBBox();
	c._stepOffset=0.1f;
	c._type=PACTOR_CAPSULE;
	c._world.createIdentity();
	c._world=_agentPos;
	c._world(3,1)+=1.0f;
	c._material = 0;
	_controller=(PhysicActorController*)Physic::getSingletonRef().createActorController(c);
	_controller->setUserData((GameObject*)this);

	if(p.getWeapon().length() > 0)
		setWeapon(GameSystem::getSingletonRef().getWeaponDB().getData(p.getWeapon()));

	if (ai)
	{
		unsigned int machineID = AISystem::getSingletonRef().getMachineID(p.getAIMachine());
		AIMachine* machine = AISystem::getSingletonRef().getMachine(machineID);
		assert(machine);
		unsigned int stateID = machine->getStateID(p.getAIState());

		_aiAgent = AISystem::getSingletonRef().createAgent(machineID, stateID, this);

		_navigator.setActorController(_controller);
		_navigator.setForwardSpeed(_template->getForwardSpeed());
		_navigator.setBackwardSpeed(_template->getBackwardSpeed());
		_navigator.setRotationSpeed(_template->getRotationSpeed());
		_navigator.setPosition(pos);
		_navigator.setOrientation(orientation);
	}
}

GameCharacter::~GameCharacter()
{
	HUD::getSingletonRef().unregisterActor(_hud);
	delete _hud;

	if (!_isPlayer)
		AISystem::getSingletonRef().destroyAgent(_aiAgent);

	_inst->sleep();
	Renderer::getSingletonRef().remRenderable(_inst);
	delete _inst;
	delete _controller;
}

void GameCharacter::changeState(GameCharacterState s)
{
	if(s!=_currentState)
	{
		_currentState=s;
		_inst->playAnimation(g_GameCharacterStateAnims[s],true);

		switch (s)
		{
		case GCS_STAND:
		case GCS_ORIENTATING:
		case GCS_DEAD:
			_walking = _running = false;
			break;

		case GCS_WALKING:
			_walking = true;
			_running = false;
			break;

		case GCS_RUNNING:
			_walking = false;
			_running = true;
			break;
		}
	}
}

void GameCharacter::update(float time)
{
	if (_navigator.isMoving())
	{
		int oldx, oldy, nx, ny;
		_navigator.getPosition(oldx, oldy);

		if (!_navigator.update(time))
		{
			//		Navigator has finished moving actor
			AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_ENDMOVE, _aiAgent, _aiAgent));
			changeState(GCS_STAND);
		}

		_agentPos = _navigator.getPosition3D();
		_agentDir.x = sinf(_navigator.getOrientation() * M_PI / 180.0f);
		_agentDir.y = 0;
		_agentDir.z = cosf(_navigator.getOrientation() * M_PI / 180.0f);

		_navigator.getPosition(nx, ny);
		if (oldx != nx || oldy != ny)
			onMove(oldx, oldy, nx, ny);
	}

	_updateTime += time;

	while (_updateTime >= AI_REFRESH_TIME)
	{
		updateSensors();
		AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_UPDATE, _aiAgent, _aiAgent));

		_updateTime -= AI_REFRESH_TIME;
	}

	makeWorldMatrix();
	_inst->setWorldMatrix(_world);

}

void GameCharacter::makeWorldMatrix()
{
	const BBox3& b(_template->getActor()->getBBox());

	Vector3 p(_navigator.getPosition().x,
		-b.getMin().y,
		_navigator.getPosition().y);

	_world.createRotateY(_navigator.getOrientation()*M_PI/180.0f);
	_world=p;
	_box.transformFromBox(b,_world);

	_hud->updatePosition(p);
}

void GameCharacter::setWeapon(Weapon* w)
{
	if(_weapon)			delete _weapon;
	_weapon=w->createInstance(this);
}

void GameCharacter::idle()
{
	changeState(GCS_STAND);
}

bool GameCharacter::goTo(const Vector3& pos, const Vector3& dir, GameCharacterState state)
{
	Vector2 goal(pos.x, pos.z);
	Vector2 d(dir.x, dir.z);
	d.normalize();

	float dOrient = acosf(d.y) * 180.0f / M_PI;
	if (d.x < 0.0f)
		dOrient=FloatMakeNegative(dOrient);

	if (_navigator.enableGoal(goal, true))
	{
		changeState(state);
		_navigator.enableOrientation(dOrient);
		return true;
	}
	else if (_navigator.enableOrientation(dOrient))
	{
		changeState(GCS_ORIENTATING);
		return true;
	}
	else
	{
		changeState(GCS_STAND);
		return false;
	}
}

bool GameCharacter::walkTo(const Vector3& pos, const Vector3& dir)
{
	return goTo(pos, dir, GCS_WALKING);
}

bool GameCharacter::runTo(const Vector3& pos, const Vector3& dir)
{
	return goTo(pos, dir, GCS_RUNNING);
}

void GameCharacter::orientTo(const Vector3& dir)
{
	Vector2 d(dir.x, dir.z);
	d.normalize();

	float dOrient=acosf(d.y) * 180.0f / M_PI;
	if (d.x < 0.0f)
		dOrient = FloatMakeNegative(dOrient);

	if (_navigator.enableOrientation(dOrient))
		changeState(GCS_ORIENTATING);
	else
		changeState(GCS_STAND);
}

void GameCharacter::goHome()
{
	if (_navigator.enableGoal(_home, true))
		changeState(GCS_RUNNING);
	else
		changeState(GCS_STAND);
}

void GameCharacter::fire()
{
	_weapon->fire();
}

void GameCharacter::updateSensors()
{
	//		Visibility

	bool see = false;
	Vector3 dir(GameSystem::getSingletonRef().getPlayer().getAgentPosition() - _agentPos);

	float lg = dir.getLength();
	if (lg < _template->getVisibilityDistance())
	{
		dir /= lg;
		float ca = dir*_agentDir;

		if (ca >= cosf(_template->getFOV()*0.5f))
		{
			RayCastInfos rc;
			rc._origin = _agentPos;
			rc._dir = dir;
			rc._range = lg;
			rc._flags = RAYCAST_SCENE;
			see = !Physic::getSingletonRef().testRay(rc);
		}
	}

	if (see && !_seePlayer)
	{
		_seePlayer = true;
		AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_SEEPLAYER, _aiAgent, _aiAgent));
	}
	else if (!see && _seePlayer)
	{
		_seePlayer = false;
		AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_LOSTPLAYER, _aiAgent, _aiAgent));
	}
}

void GameCharacter::onAddToScene()
{
	int x,y;
	_navigator.getPosition(x, y);

	GameActorUpdater::getSingletonRef().addActor(this);
//	Navigation::getSingletonRef().getMap().unsetFlag(x, y, NAV_WAKABLE);
}

void GameCharacter::onRemFromScene()
{
	int x, y;
	_navigator.getPosition(x, y);

	GameActorUpdater::getSingletonRef().remActor(this);
//	Navigation::getSingletonRef().getMap().setFlag(x, y, NAV_WAKABLE);
}

void GameCharacter::onMove(int oldx, int oldy, int newx, int newy)
{
//	Navigation::getSingletonRef().getMap().unsetFlag(oldx, oldy, NAV_WAKABLE);
//	Navigation::getSingletonRef().getMap().setFlag(newx, newy, NAV_WAKABLE);
}

void GameCharacter::onHit(const Hit& h)
{
	if (!_isPlayer)
	{
		AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_HIT,
			(h._from && h._from->getType()==ACTOR) ? ((GameCharacter*)h._from)->getAgentID() : 0xffffffff,
			_aiAgent, *((int*)&h._damage), 0));
	}

	_life -= h._damage * SystemValues::getSingletonRef().getAIDamageFactor();
	if (_life < 0.0f && !_dead)
		dies();

	_hud->updateLife(smax(0.0f, _life));
}

void GameCharacter::dies()
{
	_navigator.disableGoal();
	_navigator.disableOrientation();

	AISystem::getSingletonRef().postEvent(AIEvent(AIEVT_DEAD, _aiAgent, _aiAgent));
	AISystem::getSingletonRef().destroyAgent(_aiAgent);

	ActionServer::getSingletonRef().addDelayedAction(new AddRemObject(this,true,true),
		SystemValues::getSingletonRef().getDeadActorStayOnMapTime());

	_dead = true;

	if (SystemValues::getSingletonRef().getDemoMode())
		ActionServer::getSingletonRef().addDelayedAction(new ActionCreateRandomCharacter, 5.0f);
}