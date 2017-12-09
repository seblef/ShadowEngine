
#include "stdafx.h"
#include "PR_Point.h"
#include "Particle.h"

#define PRPOINT_PARTICLE_COUNT					1024

PR_Point::PR_Point(IVideoDevice* d) : ParticleRenderer(d), _lastBlend(0)
{
	_vBuffer=d->createVertexBuffer(PRPOINT_PARTICLE_COUNT,VX_BASE,BU_DYNAMIC);
	_vertices=new VxBase[PRPOINT_PARTICLE_COUNT];
	_whiteTexture=d->createTexture("Textures/Default/Diffuse.bmp");
}

PR_Point::~PR_Point()
{
	delete _whiteTexture;
	delete[] _vertices;
}

void PR_Point::addParticle(Particle& p, const Camera& c)
{
	VxBase& v(_vertices[_particleCount]);

	v.pos=p._pos;
	v.color=p._color;

	_lastBlend=p._material->getBlendState();

	++_particleCount;
	if(_particleCount >= PRPOINT_PARTICLE_COUNT)
		renderParticles();
}

void PR_Point::renderParticles()
{
	if(_particleCount > 0)
	{
		_vBuffer->set();
		_vBuffer->fill(_vertices,_particleCount);
		_device->setInputLayer(VX_BASE);
		_device->setPrimitiveType(PT_POINTLIST);
		_device->setBlendState(_lastBlend);
		_whiteTexture->set(0);
		
		_device->render(_particleCount);

		_particleCount=0;
	}
}