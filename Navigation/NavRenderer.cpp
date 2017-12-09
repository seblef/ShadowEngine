
#include "stdafx.h"
#include "NavRenderer.h"
#include "Navigation.h"

extern ofstream	g_MediaOut;

NavRenderer::NavRenderer(IVideoDevice *d) : _device(d), _vertices(1024), _path(0),
	_mapEnable(false)
{
	_shader=d->createShader("Dev/Nav");
	_vBuffer=d->createVertexBuffer(3072,VX_BASE,BU_DYNAMIC);
	_blendState=d->createBlendState(true,BLEND_ONE,BLEND_ONE);
	_renderState=d->createRenderState(CULL_NONE);
	_depthState=d->createDepthStencilState(false,false,COMP_ALWAYS);
}

NavRenderer::~NavRenderer()
{
	delete _shader;
	delete _vBuffer;
	_device->destroyBlendState(_blendState);
	_device->destroyDepthStencilState(_depthState);
	_device->destroyRenderState(_renderState);
}

void NavRenderer::renderNavMap()
{
	_device->resetRenderTargets();
	_shader->set();
	_vBuffer->set();
	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);
	_device->setDepthStencilState(_depthState);
	_device->setInputLayer(VX_BASE);
	_device->setPrimitiveType(PT_TRIANGLELIST);

	const NavMap& nm(Navigation::getSingletonRef().getMap());

	float sx=2.0f / (float)(nm.getWidth());
	float sy=2.0f / (float)(nm.getHeight());

	_vertices.clear();
	VxBase v[6];
	float fx,fy,fx1,fy1;

	for(int i=0;i<6;++i)
	{
		v[i].pos.y=0.0f;
		v[i].color=Color::White*0.1f;
	}

	if (_mapEnable)
	{
		for (int y = 0; y < nm.getHeight(); ++y)
			for (int x = 0; x < nm.getWidth(); ++x)
			{
				if (nm.getMap(x, y))
				{
					fx = (float)x;
					fy = (float)y;
					fx1 = fx + 1.0f;
					fy1 = fy + 1.0f;

					v[0].pos.x = fx;
					v[0].pos.z = fy;

					v[1].pos.x = fx;
					v[1].pos.z = fy1;

					v[2].pos.x = fx1;
					v[2].pos.z = fy1;

					v[3].pos.x = fx;
					v[3].pos.z = fy;

					v[4].pos.x = fx1;
					v[4].pos.z = fy1;

					v[5].pos.x = fx1;
					v[5].pos.z = fy;

					_vertices.addTab(v, 6);
				}
		}
	}

	if(_path)
	{
		for(int i=0;i<6;++i)
			v[i].color=Color(0.8,1.0,0.8,1.0) * 0.1f;

		for(int i=0;i<_path->getPathLength();++i)
		{
			fx=(float)_path->getPathStep(i).getX();
			fy=(float)_path->getPathStep(i).getY();
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