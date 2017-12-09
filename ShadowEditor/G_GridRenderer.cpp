
#include "stdafx.h"
#include "G_GridRenderer.h"
#include <StdRenderer.h>

G_GridRenderer::G_GridRenderer(IVideoDevice* d) : _device(d)
{
	_shader=d->createShader("Editor/Grid");
	_vBuffer=d->createVertexBuffer(2048,VX_BASE,BU_DYNAMIC);
	_blendState=d->createBlendState(true,BLEND_ONE,BLEND_ONE);
	_depthState=d->createDepthStencilState(true,false,COMP_LESSEQUAL);
}

G_GridRenderer::~G_GridRenderer()
{
	delete _shader;
	delete _vBuffer;
	_device->destroyBlendState(_blendState);
	_device->destroyDepthStencilState(_depthState);
}

void G_GridRenderer::render()
{
	_device->resetRenderTargets();
	_device->setBlendState(_blendState);
	_device->setDepthStencilState(_depthState);
	_shader->set();
	_vBuffer->set();
	_device->setInputLayer(VX_BASE);
	_device->setPrimitiveType(PT_LINELIST);

	const SRect &r(Renderer::getSingletonRef().getVisibilityBounds());
	float xmin=(float)r.x1;
	float xmax=(float)r.x2;
	float ymin=(float)r.y1;
	float ymax=(float)r.y2;

	float x=xmin,y=ymin;
	int count;

	VxBase verts[2048];
	VxBase* v;
	
	while(x < xmax)
	{
		count=0;
		v=verts;
		while(x < xmax && count < 1024)
		{
			v[0].pos.x=x;
			v[0].pos.y=0;
			v[0].pos.z=ymin;

			v[1].pos.x=x;
			v[1].pos.y=0;
			v[1].pos.z=ymax;

			v+=2;
			x+=1.0f;
			++count;
		}

		_vBuffer->fill(verts,count*2);
		_device->render(count*2);
	}

	while(y < ymax)
	{
		count=0;
		v=verts;
		while(y < ymax && count < 1024)
		{
			v[0].pos.x=xmin;
			v[0].pos.y=0;
			v[0].pos.z=y;

			v[1].pos.x=xmax;
			v[1].pos.y=0;
			v[1].pos.z=y;

			v+=2;
			y+=1.0f;
			++count;
		}

		_vBuffer->fill(verts,count*2);
		_device->render(count*2);
	}

	_device->setPrimitiveType(PT_TRIANGLELIST);
}