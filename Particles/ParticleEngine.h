
#ifndef _PARTICLEENGINE_H_
#define _PARTICLEENGINE_H_

#include <MediaCommon.h>
#include "ParticleSystem.h"
#include "PR_Point.h"
#include "PR_Quad.h"

class ParticleEngine : public TSingleton<ParticleEngine>
{
	protected:

		IVideoDevice*				_device;
		IShader*					_stdShader;
		DepthStencilState			_dsState;
		RenderState					_rsState;
		SamplerState				_samplerState;

		PR_Point					_pointRenderer;
		PR_Quad						_quadRenderer;
		
		Particle**					_particles;

		unsigned int				_maxParticles;
		unsigned int				_particlesOnScreen;

		typedef vector<ParticleSystem*>		PSVector;
		PSVector					_queue;

	public:

		ParticleEngine(unsigned int maxParticles, IVideoDevice *d);
		~ParticleEngine();

		unsigned int				getMaxParticles() const				{ return _maxParticles; }
		unsigned int				getParticlesOnScreen() const		{ return _particlesOnScreen; }

		void						enqueueSystem(ParticleSystem *s)	{ _queue.push_back(s); }

		void						draw(const Camera& c);

		PR_Point*					getPointRenderer()					{ return &_pointRenderer; }
		PR_Quad*					getQuadRenderer()					{ return &_quadRenderer; }
		ParticleRenderer*			getRenderer(const string& r);

		IVideoDevice*				getVideoDevice() const				{ return _device; }
};

#endif