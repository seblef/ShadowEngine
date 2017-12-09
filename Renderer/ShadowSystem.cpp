
#include "stdafx.h"
#include "ShadowSystem.h"
#include "MeshRenderer.h"
#include "ActorRenderer.h"
#include "ActorSkinnedRenderer.h"
#include "LightSpot.h"
#include "LightArea.h"
#include "Renderer.h"

extern ofstream g_MediaOut;

ShadowSystem::ShadowSystem(IVideoDevice* device) : _device(device), _base(device), _global(0)
{
	_dsState=device->createDepthStencilState(true,true,COMP_LESSEQUAL);
	_renderState=device->createRenderState(CULL_FRONT);
	_blendState=device->createBlendState(false,BLEND_ONE,BLEND_ONE);
}

ShadowSystem::~ShadowSystem()
{
	_device->destroyDepthStencilState(_dsState);
	_device->destroyBlendState(_blendState);
	_device->destroyRenderState(_renderState);
}

void ShadowSystem::renderShadowMaps()
{
	_device->setDepthStencilState(_dsState);
	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);
	_device->setPrimitiveType(PT_TRIANGLELIST);

	Camera *old_cam=Renderer::getSingletonRef().getCurrentCamera();
	Camera cam;
	IShadowMap *m;

	if(_global)
	{
		m=_base.getShadowMap(1024);
		_global->setShadowMap(m);
		getGlobalLightCamera(cam);
		cam.buildMatrices();
		_global->setShadowViewProjMatrix(cam.getViewProjMatrix());
		render(m,cam);
	}

	LightVector::iterator l(_lights.begin());
	for(;l!=_lights.end();++l)
	{
		if((*l)->getLightType()==Light::LT_SPOT)
			getSpotLightCamera(*l,cam);
		else if((*l)->getLightType()==Light::LT_AREA)
			getAreaLightCamera(*l,cam);
		else
			continue;

		m=_base.getShadowMap((*l)->getShadowMapSize());
		if(m)
		{
			cam.buildMatrices();
			(*l)->setShadowMap(m);
			(*l)->setShadowViewProjMatrix(cam.getViewProjMatrix());
			render(m,cam);
		}
	}

	Renderer::getSingletonRef().setCurrentCamera(old_cam);
}

void ShadowSystem::render(IShadowMap* m, Camera& c) const
{
	ViewFrustum vf(c.getViewProjMatrix());
	Renderer::getSingletonRef().setCurrentCamera(&c);
	m->setAsRenderTarget();
	_device->clearDepthStencil();

	MeshRenderer::getSingletonRef().renderShadows(vf);
	ActorRenderer::getSingletonRef().renderShadows(vf);
	ActorSkinnedRenderer::getSingletonRef().renderShadows(vf);
}

void ShadowSystem::getGlobalLightCamera(Camera& c) const
{
	const SRect& r(Renderer::getSingletonRef().getVisibilityBounds());

	c.setCameraType(CT_ORTHO);
	c.setCenter(Vector2(512,512));
	c.setTarget(Vector3(0.5f * (float)(r.x1+r.x2),0,0.5f * (float)(r.y1+r.y2)));
	c.setPosition(c.getTarget() - ((const Vector3&)_global->getDirection()) * 50.0f);
	c.setZMin(0.1f);
	c.setZMax(100.0f);
	c.setZoom(1);
	c.buildMatrices();

	const Matrix4& vp(c.getViewProjMatrix());
	float max_d=0;
	Vector3 v[4]={
		Vector3((float)r.x1,0,(float)r.y1),
		Vector3((float)r.x1,0,(float)r.y2),
		Vector3((float)r.x2,0,(float)r.y2),
		Vector3((float)r.x2,0,(float)r.y1)
	};

	for(int i=0;i<4;++i)
	{
		v[i]*=vp;

		float x=fabsf(v[i].x);
		float y=fabsf(v[i].y);

		if(x > max_d)			max_d=x;
		if(y > max_d)			max_d=y;
	}

	c.setZoom(1.25f * (max_d == 0.0f ? 1.0f : 0.9f / max_d));
}

void ShadowSystem::getSpotLightCamera(Light *l, Camera& c) const
{
	const LightSpot* s=(const LightSpot*)l;

	c.setCameraType(CT_PERSPECTIVE);
	c.setZMin(s->getNearZ());
	c.setZMax(l->getRange());
	c.setFOV(s->getFarAngle());
	c.setPosition(l->getWorldMatrix());
	c.setTarget(c.getPosition() + l->getDirection());
	c.setCenter(Vector2(0.5f * (float)l->getShadowMapSize(),0.5f * (float)l->getShadowMapSize()));
}

void ShadowSystem::getAreaLightCamera(Light* l, Camera& c) const
{
	const LightArea* a=(const LightArea*)l;
	float dist;
	float angle=a->getFarAngle() * M_PI / 360.0f;
	float cf=cosf(angle);
	float sf=sinf(angle);
	float div=cf/sf;

	dist=a->getAreaHeight() * div * 0.5f;
	Vector3 pos(l->getWorldMatrix());
	pos-=l->getDirection() * dist;

	Vector3 up(Vector3::YAxisVector);
	up.transformNoTranslation(l->getWorldMatrix());
	up.normalize();

	c.setCameraType(CT_PERSPECTIVE);
	c.setZMin(dist);
	c.setZMax(dist + a->getRange());
	c.setFOV(a->getFarAngle());
	c.setUpVector(up);
	c.setPosition(pos);
	c.setTarget(pos + l->getDirection());
	c.setCenter(Vector2(0.5f*(float)l->getShadowMapSize(),0.5f*(float)l->getShadowMapSize()));
}

void ShadowSystem::endRender()
{
	LightVector::iterator l(_lights.begin());
	for(;l!=_lights.end();++l)
		(*l)->setShadowMap(0);

	_base.clear();
	_lights.clear();
}