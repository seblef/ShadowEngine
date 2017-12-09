
#ifndef _SHADOWSYSTEM_H_
#define _SHADOWSYSTEM_H_

#include <Core.h>
#include "Light.h"
#include "GlobalLight.h"
#include "ShadowMapsBase.h"

using namespace Core;

class ShadowSystem : public TSingleton<ShadowSystem>
{
protected:

	typedef vector<Light*>		LightVector;

	IVideoDevice*				_device;
	ShadowMapsBase				_base;

	BlendState					_blendState;
	RenderState					_renderState;
	DepthStencilState			_dsState;

	LightVector					_lights;
	LightGlobal*				_global;

	void						getGlobalLightCamera(Camera& c) const;
	void						getSpotLightCamera(Light* l, Camera& c) const;
	void						getAreaLightCamera(Light* l, Camera& c) const;
	void						render(IShadowMap* m, Camera& c) const;

public:

	ShadowSystem(IVideoDevice* device);
	~ShadowSystem();

	void						setGlobalLight(LightGlobal* g)	{ _global=g; }

	void						enqueueLight(Light *l)			{ _lights.push_back(l); }
	void						renderShadowMaps();
	void						endRender();

	Light*						getLight(int n)													{ return _lights[n]; }
	IShadowMap*					getShadowMap(ShadowMapsBase::MapSize size, int num) const		{ return _base.getShadowMap(size,num); }
};

#endif