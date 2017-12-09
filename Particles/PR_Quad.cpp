
#include "stdafx.h"
#include "PR_Quad.h"
#include "Particle.h"
#include "ParticleTextureSet.h"

extern ofstream		g_MediaOut;

#define PRQUAD_PARTICLE_COUNT			256

PR_Quad::PR_Quad(IVideoDevice* d) : ParticleRenderer(d), _lastTexture(0), _lastBlend(0)
{
	unsigned short ind[PRQUAD_PARTICLE_COUNT*6];

	for(unsigned int i=0;i<PRQUAD_PARTICLE_COUNT;++i)
	{
		ind[i*6]=i*4;		ind[i*6+1]=i*4+1;		ind[i*6+2]=i*4+2;
		ind[i*6+3]=i*4;		ind[i*6+4]=i*4+2;		ind[i*6+5]=i*4+3;
	}

	_vBuffer=d->createVertexBuffer(PRQUAD_PARTICLE_COUNT*4,VX_BASE,BU_DYNAMIC);
	_indexBuffer=d->createIndexBuffer(PRQUAD_PARTICLE_COUNT*6,IDX_16BITS,BU_DEFAULT,ind);
	_vertices=new VxBase[PRQUAD_PARTICLE_COUNT*4];
	_particleCount=0;
}

PR_Quad::~PR_Quad()
{
	delete[] _vertices;
}

void PR_Quad::addParticle(Particle& p, const Camera& c)
{
	ITexture *t=p._material->getTexture(p._percent);

	if(t)
	{
		if(t!=_lastTexture || _particleCount==PRQUAD_PARTICLE_COUNT)
			renderParticles();

		_lastTexture=t;
		_lastBlend=p._material->getBlendState();


		Vector3 bilX(c.getXAxis() * p._size.x);
		Vector3 bilY(c.getYAxis() * p._size.y);

		VxBase *v=&_vertices[_particleCount*4];
		v->color=p._color;
		v->uv=Vector2::UnitVector;
		v->pos=p._pos + bilX + bilY;
		++v;

		v->color=p._color;
		v->uv=Vector2::YAxisVector;
		v->pos=p._pos - bilX + bilY;
		++v;

		v->color=p._color;
		v->uv=Vector2::NullVector;
		v->pos=p._pos - bilX - bilY;
		++v;

		v->color=p._color;
		v->uv=Vector2::XAxisVector;
		v->pos=p._pos + bilX - bilY;

		++_particleCount;
	}
}

void PR_Quad::renderParticles()
{
	if(_lastTexture && _particleCount > 0)
	{
		_device->setBlendState(_lastBlend);

		_vBuffer->set();
		_indexBuffer->set();
		_vBuffer->fill(_vertices,_particleCount * 4);
		_lastTexture->set(0);
		_device->setInputLayer(VX_BASE);
		_device->setPrimitiveType(PT_TRIANGLELIST);
		_device->renderIndexed(_particleCount*6);

		_particleCount=0;
		_lastTexture=0;
	}
}
