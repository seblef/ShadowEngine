
#include "stdafx.h"
#include "ParticleEmitter.h"
#include "ParticleEngine.h"

extern ofstream g_MediaOut;

ParticleEmitter::ParticleEmitter(ParticleRenderer* r, ParticleMaterial* m,
								 unsigned int maxParticles, const Vector3& grav,
								 const ParticleEmitterParams& p1, const ParticleEmitterParams& p2,
								 float emissionStep) : 
			_renderer(r), _material(m), _params1(p1), _params2(p2),
			_maxParticles(maxParticles), _gravity(grav),
			_particleCount(0), _worldMatrix(Matrix4::Identity),
			_emissionRate(emissionStep), _emissionCounter(0),
			_worldBBox(BBox3::BoundBox)
{
	initEmitter();
}

ParticleEmitter::ParticleEmitter(const ParticleEmitterTemplate& t) :
	_material(t.getMaterial()),
	_params1(t.getParameters1()), _params2(t.getParameters2()),
	_maxParticles(t.getMaxParticles()), _gravity(t.getGravity()),
	_emissionRate(t.getEmissionRate()),
	_particleCount(0), _worldMatrix(Matrix4::Identity),
	_emissionCounter(0),
	_worldBBox(BBox3::BoundBox)

{
	_renderer = ParticleEngine::getSingletonRef().getRenderer(t.getRenderer());
	initEmitter();
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] _particles;
	delete[] _freeParticles;
	delete[] _usedParticles;
}

void ParticleEmitter::initEmitter()
{
	_particles = new Particle[_maxParticles];
	_freeParticles = new Particle*[_maxParticles];
	_usedParticles = new Particle*[_maxParticles];

	for (unsigned int i = 0; i<_maxParticles; ++i)
	{
		_particles[i]._life = -1.0f;
		_particles[i]._renderer = _renderer;
		_particles[i]._material = _material;

		_freeParticles[i] = &_particles[i];
	}
}

Particle* ParticleEmitter::getNextParticle()
{
	Particle *p=0;
	if(_particleCount < _maxParticles)
	{
		p=_freeParticles[_particleCount];
		_usedParticles[_particleCount]=p;
		++_particleCount;
	}

	return p;
}

void ParticleEmitter::update(float time, const Camera& c)
{
	_emissionCounter+=time * _emissionRate;
	while(_emissionCounter >= 1.0f)
	{
		_emissionCounter-=1.0f;
		Particle *p=getNextParticle();

		if(p)
			emit(*p);
	}

	Vector3 momentum;
	const Vector3& camPos(c.getPosition());

	unsigned int currentParticle=_particleCount;
	unsigned int putPos=0;

	Vector3 pos(_worldMatrix);
	_worldBBox.setMin(pos);
	_worldBBox.setMax(pos);

	for(unsigned int i=0;i<currentParticle;++i)
	{
		Particle *p=_usedParticles[i];

		p->_life-=time;
		if(p->_life > 0.0f)
		{
			momentum=p->_vel;
			momentum*=p->_mass;

			p->_pos+=momentum * time;

			p->_color+=p->_colorCounter * time;
			p->_size+=p->_sizeCounter * time;
			p->_percent+=p->_percentCounter * time;

			p->_vel*= 1.0f - (p->_friction * time);
			p->_vel+= _gravity * time * time;

			p->_distFromCam=camPos.getDistanceFromSqr(p->_pos);

			_worldBBox << p->_pos;

			_usedParticles[putPos]=p;
			++putPos;
		}
		else
		{
			--_particleCount;
			_freeParticles[_particleCount]=p;
		}
	}
}

void ParticleEmitter::collectParticles(Particle** p, unsigned int& count, unsigned int max_count)
{
	unsigned int c=smin(_particleCount,max_count - count);
	for(unsigned int i=0;i<c;++i)
	{
		p[count]=_usedParticles[i];
		++count;
	}
}

void ParticleEmitter::initParticle(Particle& p) const
{
	p._life=randomScalar(_params1.getLife(),_params2.getLife());
	p._percent = 0;
	if (p._life > 0)
		p._percentCounter = 1.0f / p._life;
	else
		p._percentCounter = 0;

	p._mass=randomScalar(_params1.getMass(),_params2.getMass());
	p._friction=randomScalar(_params1.getFriction(),_params2.getFriction());

	randomVector(p._vel,_params1.getVelocity(),_params2.getVelocity());
	
	randomColor(p._color,_params1.getStartColor(),_params2.getStartColor());
	randomCounterColor(p._colorCounter,p._color,_params1.getEndColor(),_params2.getEndColor(),p._life);

	randomVector(p._size,_params1.getStartSize(),_params2.getStartSize());
	randomCounterVector(p._sizeCounter,p._size,_params1.getEndSize(),_params2.getEndSize(),p._life);
}


void ParticleEmitter::setMaxParticles(unsigned int mp)
{
	if (mp > _maxParticles)
	{
		delete[] _particles;
		delete[] _freeParticles;
		delete[] _usedParticles;

		_particles = new Particle[mp];
		_freeParticles = new Particle*[mp];
		_usedParticles = new Particle*[mp];

		for (unsigned int i = 0; i < mp; ++i)
		{
			_particles[i]._life = -1.0f;
			_particles[i]._renderer = _renderer;
			_particles[i]._material = _material;

			_freeParticles[i] = &_particles[i];
		}

		_particleCount = 0;
	}

	_maxParticles = mp;
}

void ParticleEmitter::setRenderer(ParticleRenderer *r)
{
	_renderer = r;
	for (unsigned int i = 0; i < _maxParticles; ++i)
		_particles[i]._renderer = r;
}

void ParticleEmitter::setMaterial(ParticleMaterial* m)
{
	if (_material)
		delete _material;

	_material = m;
	for (unsigned int i = 0; i < _maxParticles; ++i)
		_particles[i]._material = m;
}