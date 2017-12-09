
#ifndef _PR_POINT_H_
#define _PR_POINT_H_

#include "ParticleRenderer.h"

class PR_Point : public ParticleRenderer
{
	protected:

		VxBase*				_vertices;
		BlendState			_lastBlend;
		ITexture*			_whiteTexture;

	public:

		PR_Point(IVideoDevice *d);
		~PR_Point();

		const char*			getRendererName() const					{ return "point"; }

		void				addParticle(Particle& p, const Camera& c);
		void				renderParticles();

};

#endif