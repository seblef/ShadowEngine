
#include "stdafx.h"
#include "ParticleEngine.h"
#include "ParticleRenderer.h"
#include "ParticleTextureStore.h"
#include <algorithm>

extern ofstream g_MediaOut;

#define PARTICLE_STEP_TIME				(0.05f)

ParticleEngine::ParticleEngine(unsigned int maxParticles, IVideoDevice *d) : _device(d),
	_particlesOnScreen(0), _maxParticles(maxParticles),
	_pointRenderer(d), _quadRenderer(d)
{
	new ParticleTextureStore(d);

	_dsState=d->createDepthStencilState(true,false,COMP_LESSEQUAL);
	_rsState=d->createRenderState(CULL_NONE);
	_samplerState=d->createSamplerState(FILTER_TRILINEAR);
	_stdShader=d->createShader("Base/Particles");

	_particles=new Particle*[_maxParticles];
}

ParticleEngine::~ParticleEngine()
{
	_device->destroyDepthStencilState(_dsState);
	_device->destroyRenderState(_rsState);
	_device->destroySamplerState(_samplerState);
	delete _stdShader;

	delete[] _particles;

	ParticleTextureStore::deleteSingleton();
}

static inline bool ParticleSortDistance(Particle *o1, Particle *o2) 
{ 
	return o1->_distFromCam > o2->_distFromCam;
}

void ParticleEngine::draw(const Camera& c)
{
	_particlesOnScreen=0;
	PSVector::iterator s(_queue.begin());
	for(;s!=_queue.end();++s)
		(*s)->collectParticles(_particles,_particlesOnScreen,_maxParticles);

	_queue.clear();

	if(_particlesOnScreen==0)
		return;

	sort(_particles,_particles + _particlesOnScreen,ParticleSortDistance);

	_device->setDepthStencilState(_dsState);
	_device->setRenderState(_rsState);
	_device->setSamplerState(0,_samplerState);
	_stdShader->set();

	unsigned int i=0;
	ParticleRenderer *r;
	while(i < _particlesOnScreen)
	{
		r=_particles[i]->_renderer;
		while(i < _particlesOnScreen && _particles[i]->_renderer==r)
		{
			r->addParticle(*_particles[i],c);
			++i;
		}

		r->renderParticles();
	}
}

ParticleRenderer* ParticleEngine::getRenderer(const string& r)
{
	if (r == "quad")
		return &_quadRenderer;
	else
		return &_pointRenderer;
}