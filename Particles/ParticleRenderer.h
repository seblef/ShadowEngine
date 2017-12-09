
#ifndef _PARTICLERENDERER_H_
#define _PARTICLERENDERER_H_

#include <MediaCommon.h>

class Particle;

class ParticleRenderer
{
	protected:

		IVideoDevice*			_device;
		IVertexBuffer*			_vBuffer;
		int						_particleCount;

	public:

		ParticleRenderer(IVideoDevice *dev) : _device(dev), _particleCount(0)			{}
		virtual ~ParticleRenderer();

		virtual const char*		getRendererName() const									{ return "null"; }

		virtual void			addParticle(Particle& p, const Camera& c)=0;
		virtual void			renderParticles()=0;
};

#endif