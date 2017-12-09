
#include "stdafx.h"
#include "GlobalLight.h"
#include "ShadowSystem.h"

LightGlobal::LightGlobal(IVideoDevice *device, ShadowMapBuffer* shadowBuffer) : _device(device), _enable(false), _shadowBuffer(shadowBuffer)
{
	_shader = device->createShader(_T("Lights/Global/Ambient"));
	_DState=device->createDepthStencilState(false,false,COMP_ALWAYS);
	_CBuffer=device->createConstantBuffer(sizeof(GlobalBuffer_t) / (4*sizeof(float)),2);
	_blendState=device->createBlendState(false,BLEND_ONE,BLEND_ZERO);

	_lBuffer._ambient=Color::Black;
}

LightGlobal::~LightGlobal()
{
	_device->destroyDepthStencilState(_DState);
	_device->destroyBlendState(_blendState);
	delete _shader;
	delete _CBuffer;
}

void LightGlobal::enableGlobalLight(const Color& c, const Vector3& d)
{
	delete _shader;
	_shader = _device->createShader("Lights/Global/Dir");

	_lBuffer._color=c;
	_lBuffer._dir=d;
	_enable=true;

	ShadowSystem::getSingletonRef().setGlobalLight(this);
}

void LightGlobal::disableGlobalLight()
{
	delete _shader;
	_shader = _device->createShader("Lights/Global/Ambient");
	_enable=false;

	ShadowSystem::getSingletonRef().setGlobalLight(0);
}

void LightGlobal::applyLight() const
{
	_CBuffer->set(ST_PIXEL);
	_CBuffer->fill((const void*)&_lBuffer);
	_shader->set();
	_device->setBlendState(_blendState);
	_device->setDepthStencilState(_DState);

	if(_enable)
	{
		_shadowBuffer->setupLight(_shadowMap,6.0f,_shadowViewProj);
		_shadowMap->set(7);
	}

	_device->renderFullscreenQuad();
}