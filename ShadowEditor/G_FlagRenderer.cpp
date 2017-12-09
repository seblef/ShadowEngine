

#include "stdafx.h"
#include "G_FlagRenderer.h"
#include "G_EditorApp.h"
#include <StdRenderer.h>

extern ofstream g_MediaOut;

G_FlagRenderer::G_FlagRenderer(IVideoDevice* d) : _device(d), _flags(0), _vertices(1024)
{
	_shader=d->createShader("Editor/Flags");
	_vBuffer=d->createVertexBuffer(3072,VX_BASE,BU_DYNAMIC);
	_blendState=d->createBlendState(true,BLEND_ONE,BLEND_ONE);
	_depthState=d->createDepthStencilState(true,false,COMP_LESSEQUAL);
	_renderState=d->createRenderState(CULL_NONE);
}

G_FlagRenderer::~G_FlagRenderer()
{
	delete _shader;
	delete _vBuffer;
	_device->destroyBlendState(_blendState);
	_device->destroyDepthStencilState(_depthState);
	_device->destroyRenderState(_renderState);
}

void G_FlagRenderer::render()
{
	_device->resetRenderTargets();
	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);
	_device->setDepthStencilState(_depthState);
	_shader->set();
	_vBuffer->set();
	_device->setInputLayer(VX_BASE);
	_device->setPrimitiveType(PT_TRIANGLELIST);

	const SRect &r(Renderer::getSingletonRef().getVisibilityBounds());
	float fx,fy,fx1,fy1;
	int sx,ex,sy,ey;

	sminmax(r.x1,r.x2,sx,ex);
	sminmax(r.y1,r.y2,sy,ey);

	sclamp(sx,0,G_EditorApp::getSingletonRef().getMap().getGround().getWidth()-1);
	sclamp(ex,0,G_EditorApp::getSingletonRef().getMap().getGround().getWidth()-1);

	sclamp(sy,0,G_EditorApp::getSingletonRef().getMap().getGround().getHeight()-1);
	sclamp(ey,0,G_EditorApp::getSingletonRef().getMap().getGround().getHeight()-1);

	_vertices.clear();
	VxBase v[6];
	for(int i=0;i<6;++i)
	{
		v[i].color=Color::Green * 0.2f;
		v[i].pos.y=0.1f;
	}

	const G_Ground& g(G_EditorApp::getSingletonRef().getMap().getGround());
	for(int y=sy;y<=ey;++y)
		for(int x=sx;x<=ex;++x)
		{
			if(g.getFlag(x,y,_flags))
			{
				fx=(int)x;
				fy=(int)y;
				fx1=fx+1.0f;
				fy1=fy+1.0f;

				v[0].pos.x=fx;
				v[0].pos.z=fy;

				v[1].pos.x=fx;
				v[1].pos.z=fy1;

				v[2].pos.x=fx1;
				v[2].pos.z=fy1;

				v[3].pos.x=fx;
				v[3].pos.z=fy;

				v[4].pos.x=fx1;
				v[4].pos.z=fy1;

				v[5].pos.x=fx1;
				v[5].pos.z=fy;

				_vertices.addTab(v,6);
			}
		}

	int c=_vertices.getCount() / 3072;
	int q=_vertices.getCount() % 3072;
	int cur=0;

	for(int i=0;i<c;++i,cur+=3072)
	{
		_vBuffer->fill(_vertices.getBuffer() + cur,3072);
		_device->render(3072);
	}

	if(q > 0)
	{
		_vBuffer->fill(_vertices.getBuffer() + cur,q);
		_device->render(q);
	}
}