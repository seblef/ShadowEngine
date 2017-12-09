
#include "stdafx.h"
#include "WeaponRocket.h"
#include "WeaponRocketInstance.h"
#include "GameSystem.h"
#include <SoundLib.h>


WeaponRocket::WeaponRocket(ScriptFile& sf) : _rocketLife(2.0f), _rocketVelocity(10.0f),
	_rocketMesh(0), _rocketExplosion(0), _rocketSmoke(0),  _loaded(false),
	_rocketSmokePoint(Vector3::NullVector), _rocketSound(0)
{
	string t(sf.getToken());
	while (sf.good() && t != "end_weapon")
	{
		if (t == "rocket_mesh")				_rocketMeshFile = sf.getToken();
		else if (t == "rocket_material")	_rocketMaterialFile = sf.getToken();
		else if (t == "rocket_life")		_rocketLife = stof(sf.getToken());
		else if (t == "rocket_velocity")	_rocketVelocity = stof(sf.getToken());
		else if (t == "rocket_explosion")	_rocketExplosionName = sf.getToken();
		else if (t == "rocket_smoke")		_rocketSmoke = new ParticleSystemTemplate(sf);
		else if (t == "rocket_smoke_point")	sf.parseVector(_rocketSmokePoint);
		else if (t == "rocket_sound")		_rocketSoundFile = sf.getToken();
		else								parseToken(t, sf);

		t = sf.getToken();
	}
}

WeaponRocket::~WeaponRocket()
{
	unload();
}

void WeaponRocket::load()
{
	if (_loaded)
		return;

	if (!_rocketMeshFile.empty() && !_rocketMaterialFile.empty())
		_rocketMesh = TemplateMesh::buildMesh(_rocketMeshFile, _rocketMaterialFile, 0, 0, PSHAPE_CAPSULE);

	if (!_rocketExplosionName.empty())
	{
		_rocketExplosion = (ExplosionTemplate*)GameSystem::getSingletonRef().getEffectDB().getData(_rocketExplosionName);
		assert(_rocketExplosion);
		_rocketExplosion->load();
	}

	if (!_rocketSoundFile.empty())
		_rocketSound = SoundSystem::getSingletonRef().loadSound(_rocketSoundFile);

	Weapon::load();

	_loaded = true;
}

void WeaponRocket::unload()
{
	if (!_loaded)
		return;

	if (_rocketMesh)			delete _rocketMesh;
	if (_rocketSound)			SoundSystem::getSingletonRef().releaseSound(_rocketSound);

	_rocketMesh = 0;
	_rocketExplosion = 0;
	_rocketSound = 0;

	Weapon::unload();

	_loaded = false;
}

WeaponInstance* WeaponRocket::createInstance(GameCharacter *c)
{
	return new WeaponRocketInstance(this, c);
}