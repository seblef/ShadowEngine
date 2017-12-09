
#include "stdafx.h"
#include "LightSystem.h"
#include "LightOmniRenderer.h"
#include "LightSpotRenderer.h"
#include "LightAreaRenderer.h"
#include "ShadowSystem.h"
#include "HDAO.h"
#include "HDR.h"

LightSystem::LightSystem(IVideoDevice* device) : _shadowBuffer(device), _global(device,&_shadowBuffer), _device(device)
{
	_cLightBuffer=device->createConstantBuffer(8,2);

	_dsIn=device->createDepthStencilStateEx(true,false,COMP_GREATEREQUAL,
		true, 0xff, 0xff,
		COMP_EQUAL2,STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP,
		COMP_EQUAL2, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);
	_dsOut=device->createDepthStencilStateEx(true,false,COMP_LESSEQUAL,
		true, 0xff, 0xff,
		COMP_EQUAL2, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP,
		COMP_EQUAL2, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);

	_rsIn=device->createRenderState(CULL_FRONT);
	_rsOut=device->createRenderState(CULL_BACK);

	_samplerState = device->createSamplerState(FILTER_POINT, ADDRESS_CLAMP, ADDRESS_CLAMP);

	LightRendererCreate_t c;
	c._device=device;
	c._cLightBuffer=_cLightBuffer;
	c._shadowBuffer=&_shadowBuffer;
	c._dsIn=_dsIn;
	c._dsOut=_dsOut;
	c._rsIn=_rsIn;
	c._rsOut=_rsOut;

	_renderers[Light::LT_OMNI]=new LightOmniRenderer(c);
	_renderers[Light::LT_SPOT]=new LightSpotRenderer(c);
	_renderers[Light::LT_AREA]=new LightAreaRenderer(c);

	_blendState=device->createBlendState(true,BLEND_ONE,BLEND_ONE);

	_noBlendState = device->createBlendState(false, BLEND_ONE, BLEND_ZERO);
}

LightSystem::~LightSystem()
{
	for(int i=0;i<Light::LT_COUNT;++i)
		delete _renderers[i];

	delete _cLightBuffer;

	_device->destroyDepthStencilState(_dsIn);
	_device->destroyDepthStencilState(_dsOut);

	_device->destroyRenderState(_rsIn);
	_device->destroyRenderState(_rsOut);

	_device->destroyBlendState(_blendState);
	_device->destroySamplerState(_samplerState);

	_device->destroyBlendState(_noBlendState);
}

void LightSystem::enqueueLight(Light *l)
{
	_renderers[l->getLightType()]->enqueueLight(l);
	if(l->getCastShadows() && l->getLightType()!=Light::LT_OMNI)
		ShadowSystem::getSingletonRef().enqueueLight(l);
}

void LightSystem::applyLights(const Vector3& eyePos, GBuffer& gbuf)
{
	_shadowBuffer.set();
	_device->setRenderTarget(HDR::getSingletonRef().getAccumulationBuffer(), 0);
	_device->applyRenderTargets();
	_device->clearRenderTargets(Color(0, 0, 0, 0));
	_device->setStencilRef(1);

	gbuf.setAsTextures();

	SamplerState ss[4] = { _samplerState, _samplerState, _samplerState, _samplerState };
	_device->setSamplerState(0, 4, ss);

	_device->setInputLayer(VX_BASE);
	_device->setBlendState(_blendState);
	_cLightBuffer->set(ST_PIXEL);

	for (int i = 0; i<Light::LT_COUNT; ++i)
		_renderers[i]->applyLights(eyePos);

	HDR::getSingletonRef().setBackBuffer();
	gbuf.setAsTextures();

	HDR::getSingletonRef().getAccumulationBuffer()->set(4);
	HDAO::getSingletonRef().getHDAOTexture()->set(5);

	_device->setBlendState(_noBlendState);
	_global.applyLight();
}