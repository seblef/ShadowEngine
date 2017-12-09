
#include "stdafx.h"
#include "Renderer2D.h"
#include "R2D_FrameLoader.h"
#include "R2D_ImageRenderer.h"
#include "R2D_LineRenderer.h"
#include "R2D_RectRenderer.h"
#include "R2D_TextRenderer.h"

Renderer2D::Renderer2D(IVideoDevice* device) : _device(device), _currentFrame(0)
{
	_shader=device->createShader(_T("2D/R2D"));
	_whiteTexture=device->createTexture(_T("2D/White.bmp"));
	_blendState=device->createBlendState(true,BLEND_SRCALPHA,BLEND_INVSRCALPHA);
	_renderState=device->createRenderState(CULL_NONE);
	_depthState=device->createDepthStencilState(false,false,COMP_ALWAYS);
	_samplerState=device->createSamplerState(FILTER_BILINEAR);

	_renderers[R2D_Object::R2D_LINE]=new R2D_LineRenderer(true,_whiteTexture,device);
	_renderers[R2D_Object::R2D_RECT]=new R2D_RectRenderer(true,_whiteTexture,device);
	_renderers[R2D_Object::R2D_IMG]=new R2D_ImageRenderer(true,_whiteTexture,device);
	_renderers[R2D_Object::R2D_TEXT]=new R2D_TextRenderer(true,_whiteTexture,device);
}

Renderer2D::~Renderer2D()
{
	for(int i=0;i<R2D_Object::R2D_COUNT;++i)
		delete _renderers[i];

	delete _shader;
	delete _whiteTexture;
	_device->destroyBlendState(_blendState);
	_device->destroyDepthStencilState(_depthState);
	_device->destroyRenderState(_renderState);
	_device->destroySamplerState(_samplerState);
}

R2D_Frame* Renderer2D::loadFrame(const string& frameFile)
{
	return R2D_FrameLoader::loadFrame(frameFile,_device);
}

void Renderer2D::render()
{
	if(!_currentFrame)
		return;

	_shader->set();
	_device->resetRenderTargets();
	_device->setBlendState(_blendState);
	_device->setDepthStencilState(_depthState);
	_device->setRenderState(_renderState);
	_device->setSamplerState(0,_samplerState);
	_device->setInputLayer(VX_2D);

	for(int l=0;l<_currentFrame->getVector().size();++l)
		renderLayer(*_currentFrame->getVector()[l]);
}

void Renderer2D::renderLayer(const R2D_Layer& l) const
{
	set<R2D_Object*>::const_iterator o(l.getSet().begin());
	for(;o!=l.getSet().end();++o)
		_renderers[(*o)->getType()]->addObject(*o);

	for(int i=0;i<R2D_Object::R2D_COUNT;++i)
		_renderers[i]->execute();
}