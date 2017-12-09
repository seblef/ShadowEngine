
#include "stdafx.h"
#include "Rocket.h"
#include "WeaponRocket.h"
#include "GameCharacter.h"
#include "ActionServer.h"
#include "Explosion.h"

extern ofstream	g_MediaOut;


Rocket::Rocket(WeaponRocket* weapon, GameCharacter* owner,
	const Matrix4& originalWorld, const Vector3& direction) : _weapon(weapon),
	_owner(owner), _world(originalWorld), _rocketMesh(0), _rocketLight(0),
	_rocketPhysic(0), _rocketSmoke(0),
	_rocketSource(SoundSystem::getSingletonRef().getAudio(),1,1.0f,true)
{
	_life = weapon->getRocketLife();
	_velocity = direction * weapon->getRocketVelocity();

	if (weapon->getRocketMesh())
	{
		_rocketMesh = new MeshInstance(&weapon->getRocketMesh()->getMesh(), originalWorld, true);
		Renderer::getSingletonRef().addRenderable(_rocketMesh);

		_rocketPhysic = Physic::getSingletonRef().createAmmo(*weapon->getRocketMesh()->getPhysicGeometry(), _world);
		_rocketPhysic->setUserData((Ammo*)this);
		_rocketPhysic->addToScene();
	}

	Vector3 smokePos(weapon->getRocketSmokePoint() * originalWorld);
	_smokeWorld.createTranslate(smokePos.x, smokePos.y, smokePos.z);

	if (weapon->isFlashEnable())
	{
		LightCreate_t lc;
		lc._castShadows = false;
		lc._color = weapon->getFlashColor();
		lc._range = weapon->getFlashRange();
		lc._world = _smokeWorld;
		_rocketLight = new Light(Light::LT_OMNI, lc);

		Renderer::getSingletonRef().addRenderable(_rocketLight);
	}

	if (weapon->getRocketSmoke())
	{
		ParticleSystem *ps = new ParticleSystem(*weapon->getRocketSmoke());
		ps->setWorldMatrix(_smokeWorld);

		_rocketSmoke = new Particles(ps, _smokeWorld, false);
		_rocketSmoke->wakeUp();
		Renderer::getSingletonRef().addRenderable(_rocketSmoke);
	}

	if (weapon->getRocketSound())
	{
		_rocketSource.getSource()->setPosition(smokePos);
		_rocketSource.getSource()->setVelocity(_velocity);
		SoundSystem::getSingletonRef().play(&_rocketSource, weapon->getRocketSound());
	}
}

Rocket::~Rocket()
{
	cleanUp();
}

void Rocket::update(float time)
{
	_life -= time;
	if (_life <= 0.0f)
		explode();
	else
	{
		Vector3 delta(_velocity * time);
		_world(3, 0) += delta.x;
		_world(3, 1) += delta.y;
		_world(3, 2) += delta.z;

		_smokeWorld(3, 0) += delta.x;
		_smokeWorld(3, 1) += delta.y;
		_smokeWorld(3, 2) += delta.z;

		if (_rocketMesh)			_rocketMesh->setWorldMatrix(_world);
		if (_rocketPhysic)			_rocketPhysic->setWorldMatrix(_world);
		if (_rocketLight)			_rocketLight->setWorldMatrix(_smokeWorld);
		if (_rocketSmoke)			_rocketSmoke->setWorldMatrix(_smokeWorld);

		if (_rocketSource.isPlaying())
			_rocketSource.getSource()->setPosition(_rocketSource.getSource()->getPosition() + delta);
	}
}

void Rocket::onContact(IPhysicObject* other)
{
	if (_owner->getPhysicObject() != other)
		explode();
}

void Rocket::explode()
{
	if (_dead)
		return;

	if (_weapon->getRocketExplosion())
	{
		Vector3 p;
		p.x = _world(3, 0);
		p.y = _world(3, 1);
		p.z = _world(3, 2);
		ActionServer::getSingletonRef().addAction(new Explosion(*_weapon->getRocketExplosion(),p));

		cleanUp();
	}

	_dead = true;
}

void Rocket::cleanUp()
{
	if (_rocketPhysic)
	{
		_rocketPhysic->remFromScene();
		delete _rocketPhysic;
		_rocketPhysic = 0;
	}

	if (_rocketMesh)
	{
		Renderer::getSingletonRef().remRenderable(_rocketMesh);
		delete _rocketMesh;
		_rocketMesh = 0;
	}

	if (_rocketLight)
	{
		Renderer::getSingletonRef().remRenderable(_rocketLight);
		delete _rocketLight;
		_rocketLight = 0;
	}

	if (_rocketSmoke)
	{
		_rocketSmoke->sleep();
		Renderer::getSingletonRef().remRenderable(_rocketSmoke);
		delete _rocketSmoke;
		_rocketSmoke = 0;
	}

	if (_rocketSource.isPlaying())
		SoundSystem::getSingletonRef().stopSound(&_rocketSource);
}