
#ifndef _LIGHTSYSTEM_H_
#define _LIGHTSYSTEM_H_

#include "GlobalLight.h"
#include "Light.h"
#include "LightRenderer.h"
#include "GBuffer.h"

class LightSystem : public TSingleton<LightSystem>
{
protected:

	IVideoDevice*		_device;

	LightGlobal			_global;
	LightRenderer*		_renderers[Light::LT_COUNT];

	IConstantBuffer*	_cLightBuffer;
	ShadowMapBuffer		_shadowBuffer;
	RenderState			_rsIn;
	RenderState			_rsOut;
	DepthStencilState	_dsIn;
	DepthStencilState	_dsOut;
	BlendState			_blendState;
	SamplerState		_samplerState;

	BlendState			_noBlendState;

public:

	LightSystem(IVideoDevice* device);
	~LightSystem();

	LightGlobal&		getGlobalLight()						{ return _global; }
	const LightGlobal&	getGlobalLight() const					{ return _global; }

	void				enqueueLight(Light* l);

	void				applyLights(const Vector3& eyePos, GBuffer& gbuf);
};

#endif