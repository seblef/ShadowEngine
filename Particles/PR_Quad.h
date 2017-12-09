
#ifndef _PR_QUAD_H_
#define _PR_QUAD_H_

#include "ParticleRenderer.h"

class PR_Quad : public ParticleRenderer
{
	protected:

		VxBase*				_vertices;
		ITexture*			_lastTexture;
		IIndexBuffer*		_indexBuffer;
		BlendState			_lastBlend;

	public:

		PR_Quad(IVideoDevice* d);
		~PR_Quad();

		const char*			getRendererName() const					{ return "quad"; }

		void				addParticle(Particle& p, const Camera& c);
		void				renderParticles();
};

#endif