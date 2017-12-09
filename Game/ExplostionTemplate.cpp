
#include "stdafx.h"
#include "ExplosionTemplate.h"
#include "ObjectFlags.h"
#include <SoundLib.h>

const float ExplosionMinMass = 0.2f;
const float ExplosionMaxMass = 0.5f;
const float ExplosionMinFriction = 0.01f;
const float ExplosionMaxFriction = 0.02f;
const float ExplosionMinSizeX = 0.2f;
const float ExplosionMinSizeY = 0.2f;
const float ExplosionMaxSizeX = 0.5f;
const float ExplosionMaxSizeY = 0.5f;

const float ExplosionSmokeMinLife = 2.4f;
const float ExplosionSmokeMaxLife = 3.0f;
const float ExplosionSmokeMinMass = 0.5f;
const float ExplosionSmokeMaxMass = 0.5f;
const float ExplosionSmokeMinFriction = 0.01f;
const float ExplosionSmokeMaxFriction = 0.02f;
const Vector3 ExplosionSmokeMinVelocity(-1, 0.8, -1);
const Vector3 ExplosionSmokeMaxVelocity(1, 1, 1);
const Color ExplosionSmokeMinColor(0.8, 0.8, 0.8, 1.0);
const float ExplosionSmokeMinStartSize = 0.1f;
const float ExplosionSmokeMaxStartSize = 0.2f;
const float ExplosionSmokeMinEndSize = 0.3f;
const float ExplosionSmokeMaxEndSize = 0.5f;
const float ExplosionSmokeEmissionRate = 50.0f;
const int ExplosionSmokeMaxParticles = 200;


ExplosionTemplate::ExplosionTemplate(ScriptFile& sf) : Effect(EXPLOSION), _size(1), _life(1), _smokeDensity(1), _smokeLife(0.5f), _color(Color::White),
	_explosionColorBlend(false), _debrisSize(0.01f), _debrisMinVelocity(1.0f), _debrisMaxVelocity(10.0f), _debrisMaxAngle(M_PI_4),
	_debrisMaxAngleSpeed(M_PI), _debrisCount(5),
	_particles(0), _debrisMesh(0), _loaded(false), 
	_explosionEmitter(-1), _smokeEmitter(-1), _smokeColorBlend(false), _particleCount(5),
	_explosionSound(0), _hitEnvironment(true),
	_innerRadius(1.0f), _outerRadius(2.0f), _innerDamage(50.0f)
{
	string t(sf.getToken());
	while (t != "end_explosion")
	{
		if (t == "size")						_size = stof(sf.getToken());
		else if (t == "life")					_life = stof(sf.getToken());
		else if (t == "color")					sf.parseColor(_color);
		else if (t == "particle_count")			_particleCount = stoi(sf.getToken());
		else if (t == "smoke_density")			_smokeDensity = stof(sf.getToken());
		else if (t == "smoke_life")				_smokeLife = stof(sf.getToken());
		else if (t == "texture_dir")			_explosionTextureDir=sf.getToken();
		else if (t == "smoke_texture")			_smokeTexture = sf.getToken();
		else if (t == "debris_mesh")			_debrisMeshFile = sf.getToken();
		else if (t == "debris_material")		_debrisMaterial = sf.getToken();
		else if (t == "debris_size")			_debrisSize = stof(sf.getToken());
		else if (t == "debris_min_vel")			_debrisMinVelocity = stof(sf.getToken());
		else if (t == "debris_max_vel")			_debrisMaxVelocity = stof(sf.getToken());
		else if (t == "debris_max_angle")		_debrisMaxAngle = stof(sf.getToken());
		else if (t == "debris_max_angle_speed")	_debrisMaxAngleSpeed = stof(sf.getToken());
		else if (t == "debris_count")			_debrisCount = stoi(sf.getToken());
		else if (t == "explosion_color_blend")	_explosionColorBlend = true;
		else if (t == "smoke_color_blend")		_smokeColorBlend = true;
		else if (t == "explosion_sound")		_explosionSoundFile = sf.getToken();
		else if (t == "inner_radius")			_innerRadius = stof(sf.getToken());
		else if (t == "outer_radius")			_outerRadius = stof(sf.getToken());
		else if (t == "inner_damage")			_innerDamage = stof(sf.getToken());
		else if (t == "no_hit")					_hitEnvironment = false;

		t = sf.getToken();
	}
}

ExplosionTemplate::~ExplosionTemplate()
{
	unload();
}

void ExplosionTemplate::load()
{
	if (_loaded)		return;

	int pcount = 0;
	ParticleEmitterTemplate* emitters[2] = { 0, 0 };

	if (!_explosionTextureDir.empty())
	{
		BlendMode src = BLEND_SRCALPHA;
		BlendMode dest = BLEND_INVSRCALPHA;

		if (_explosionColorBlend)
		{
			src = BLEND_SRCCOLOR;
			dest = BLEND_INVSRCCOLOR;
		}

		ParticleMaterial *m = new ParticleMaterial(_explosionTextureDir,true,src,dest);
		ParticleEmitterParams minp(_life * 0.5f, ExplosionMinMass, ExplosionMinFriction,
			_color, _color,
			Vector3::NullVector,
			Vector3(ExplosionMinSizeX, ExplosionMinSizeY, 0),
			Vector3(_size, _size, _size));
		ParticleEmitterParams maxp(_life, ExplosionMaxMass, ExplosionMaxFriction,
			_color, _color, Vector3::NullVector,
			Vector3(ExplosionMaxSizeX, ExplosionMaxSizeY, 0),
			Vector3(_size, _size, _size));

		emitters[0] = new ParticlePointEmitterTemplate("quad", m, minp, maxp, 30,
			Vector3::NullVector, 0);
		_explosionEmitter = 0;
		++pcount;
	}

	if (!_smokeTexture.empty())
	{
		BlendMode src = BLEND_SRCALPHA;
		BlendMode dest = BLEND_INVSRCALPHA;

		if (_smokeColorBlend)
		{
			src = BLEND_SRCCOLOR;
			dest = BLEND_INVSRCCOLOR;
		}

		ParticleMaterial* m = new ParticleMaterial(_smokeTexture,false,src, dest);
		ParticleEmitterParams minp(ExplosionSmokeMinLife,
			ExplosionSmokeMinMass,
			ExplosionSmokeMinFriction,
			ExplosionSmokeMinColor,
			Color::Black,
			ExplosionSmokeMinVelocity,
			Vector3(ExplosionSmokeMinStartSize, ExplosionSmokeMinStartSize, ExplosionSmokeMinStartSize),
			Vector3(ExplosionSmokeMinEndSize, ExplosionSmokeMinEndSize, ExplosionSmokeMinEndSize));
		ParticleEmitterParams maxp(ExplosionSmokeMaxLife,
			ExplosionSmokeMaxMass,
			ExplosionSmokeMaxFriction,
			Color::White,
			Color::Black,
			ExplosionSmokeMaxVelocity,
			Vector3(ExplosionSmokeMaxStartSize, ExplosionSmokeMaxStartSize, ExplosionSmokeMaxStartSize),
			Vector3(ExplosionSmokeMaxEndSize, ExplosionSmokeMaxEndSize, ExplosionSmokeMaxEndSize));

		emitters[pcount] = new ParticlePointEmitterTemplate("quad", m, minp, maxp,
			(unsigned int)(_smokeDensity * (float)ExplosionSmokeMaxParticles),
			Vector3::NullVector, ExplosionSmokeEmissionRate * _smokeDensity);
		_smokeEmitter = pcount;
		++pcount;
	}

	_particles = new ParticleSystemTemplate(pcount, emitters);

	if (!_debrisMeshFile.empty() && !_debrisMaterial.empty())
		_debrisMesh = TemplateMesh::buildMesh(_debrisMeshFile, _debrisMaterial, 0, OF_NOCOLLISION);

	if (!_explosionSoundFile.empty())
		_explosionSound=SoundSystem::getSingletonRef().loadSound(_explosionSoundFile);

	_loaded = true;
}

void ExplosionTemplate::unload()
{
	if (!_loaded)
		return;

	delete _particles;
	if (_debrisMesh)					delete _debrisMesh;
	if (_explosionSound)				SoundSystem::getSingletonRef().releaseSound(_explosionSound);

	_particles = 0;
	_debrisMesh = 0;
	_explosionEmitter = _smokeEmitter = -1;
	_explosionSound = 0;

	_loaded = false;
}