
#include "stdafx.h"
#include "Renderer.h"
#include "RendererStats.h"
#include "GroundRenderer.h"
#include "MaterialSystem.h"
#include "MeshRenderer.h"
#include "VisibilityFactory.h"
#include "LightSystem.h"
#include "UpdateSystem.h"
#include "ActorRenderer.h"
#include "ActorSkinnedRenderer.h"
#include "ShadowSystem.h"
#include "HDAO.h"
#include "HDR.h"
#include <ParticleLib.h>

Renderer::Renderer(IVideoDevice *device, Config& cfg) : _GBuffer(device), _device(device), _currentCamera(0)
{
	new UpdateSystem;
	new GroundRenderer(device);
	new MaterialSystem(device);
	new LightSystem(device);
	new MeshRenderer(device);
	new ActorRenderer(device);
	new ActorSkinnedRenderer(device);
	new ShadowSystem(device);
	new ParticleEngine(10000, device);
	new HDAO(device, cfg);
	new HDR(device, cfg);

	_sceneInfosCS=device->createConstantBuffer(sizeof(SceneInfosBuffer) / (4*sizeof(float)),0);
	_objectInfosCS=device->createConstantBuffer(4,1);

	_visAlways=VisibilityFactory::createVisibilitySystem("all");
	_vis=VisibilityFactory::createVisibilitySystem("quad");

	_dsSolidWrite=device->createDepthStencilStateEx(true,true,COMP_LESSEQUAL,
		true,0xff,0xff,
		COMP_ALWAYS, STENCIL_KEEP, STENCIL_KEEP, STENCIL_REPLACE,
		COMP_ALWAYS, STENCIL_KEEP, STENCIL_KEEP, STENCIL_REPLACE);
	_dsSolidAdd = device->createDepthStencilState(true, false, COMP_EQUAL2);
}

Renderer::~Renderer()
{
	ParticleEngine::deleteSingleton();
	GroundRenderer::deleteSingleton();
	MaterialSystem::deleteSingleton();
	LightSystem::deleteSingleton();
	MeshRenderer::deleteSingleton();
	ActorRenderer::deleteSingleton();
	ActorSkinnedRenderer::deleteSingleton();
	UpdateSystem::deleteSingleton();
	ShadowSystem::deleteSingleton();
	HDAO::deleteSingleton();
	HDR::deleteSingleton();

	if(_g_data)			delete _g_data;

	delete _objectInfosCS;
	delete _sceneInfosCS;

	delete _visAlways;
	delete _vis;

	_device->destroyDepthStencilState(_dsSolidWrite);
	_device->destroyDepthStencilState(_dsSolidAdd);
}

void Renderer::setCurrentCamera(Camera *c)
{
	c->buildMatrices();
	_sceneInfos.setCamera(*c);
	_sceneInfosCS->fill((const void*)&_sceneInfos);
	_currentCamera=c;
}

void Renderer::update(float time, Camera *c)
{
#ifdef _OUTPUTSTATS
	RendererStats::getSingletonRef().newFrame();
#endif

	UpdateSystem::getSingletonRef().process(time);

	setCurrentCamera(c);
	visibility(*c);

//	_device->newFrame();
	_device->resetRenderTargets();

	_sceneInfosCS->set(ST_VERTEX);
	_sceneInfosCS->set(ST_PIXEL);

	_objectInfosCS->set(ST_VERTEX);
	_objectInfosCS->set(ST_PIXEL);

	ShadowSystem::getSingletonRef().renderShadowMaps();
	_device->resetRenderTargets();

	_GBuffer.setAsRenderTarget();

	_device->clearDepthStencil();
	_device->clearRenderTargets(Color::Black);

	MaterialSystem::getSingletonRef().setBuffers();
	MeshRenderer::getSingletonRef().prepareRender();
	ActorRenderer::getSingletonRef().prepareRender();
	ActorSkinnedRenderer::getSingletonRef().prepareRender();

	renderPass(false);

	HDAO::getSingletonRef().process(_GBuffer, *c);

	_device->resetRenderTargets();

	LightSystem::getSingletonRef().applyLights(c->getPosition(),_GBuffer);

	MeshRenderer::getSingletonRef().endRender();
	ActorRenderer::getSingletonRef().endRender();
	ActorSkinnedRenderer::getSingletonRef().endRender();

//	_device->resetRenderTargets();
	ParticleEngine::getSingletonRef().draw(*_currentCamera);

	HDR::getSingletonRef().process();

//	_GBuffer.copyTexture(HDAO::getSingletonRef().getHDAOTexture());
	//	_GBuffer.copyNormal();
//	_GBuffer.copyShadowMap(LightSystem::getSingletonRef().getGlobalLight().getShadowMap());
	ShadowSystem::getSingletonRef().endRender();

//	renderPass(true);

}

void Renderer::renderPass(bool add)
{
	_device->setPrimitiveType(PT_TRIANGLELIST);
	_device->setDepthStencilState(add ? _dsSolidAdd : _dsSolidWrite);
	_device->setStencilRef(1);

	ActorSkinnedRenderer::getSingletonRef().render(add);
	ActorRenderer::getSingletonRef().render(add);
	MeshRenderer::getSingletonRef().render(add);

	if(_g_data)
	{
		setObjectTransformation(Matrix4::Identity);
		GroundRenderer::getSingletonRef().renderGround(*_g_data,add);
	}
}

void Renderer::visibility(const Camera& c)
{
	ViewFrustum vf(c.getViewProjMatrix());
	vf.setPosition(c.getPosition());

	int w=65535,h=65535;
	if(_g_data)
	{
		w=_g_data->getWidth();
		h=_g_data->getHeight();
	}

	computeVisBounds(vf,w,h);

	if(_g_data)
		GroundRenderer::getSingletonRef().visibility(*_g_data,_visBounds);

	_visAlways->visibility(vf,_visBounds);
	_vis->visibility(vf,_visBounds);

}

void Renderer::computeVisBounds(const ViewFrustum& vf, int w, int h)
{
	Vector3 corners[5];
	Vector3 d[2];
	float _xmin,_xmax,_ymin,_ymax;
	int c_count=5;

	corners[0]=vf.getFarLeftDown();
	corners[1]=vf.getFarLeftUp();
	corners[2]=vf.getFarRightDown();
	corners[3]=vf.getFarRightUp();
	corners[4]=vf.getPosition();

	d[0]=corners[1] - vf.getPosition();
	d[1]=corners[3] - vf.getPosition();

	if(d[0].y < 0.0f && d[1].y < 0.0f)
	{
		c_count=4;
		Plane p(Vector3::NullVector,Vector3::YAxisVector);
		ViewFrustumPlane vfp[4][2]={
			{ VFP_BOTTOM, VFP_LEFT },
			{ VFP_LEFT, VFP_TOP },
			{ VFP_TOP, VFP_RIGHT },
			{ VFP_RIGHT, VFP_BOTTOM }};

		for(int i=0;i<4;++i)
			p.intersectWithPlanes(vf.getPlane(vfp[i][0]),vf.getPlane(vfp[i][1]),corners[i]);
	}

	_xmin=_xmax=corners[0].x;
	_ymin=_ymax=corners[0].z;

	for(int i=1;i<c_count;++i)
	{
		_xmin=smin(_xmin,corners[i].x);
		_xmax=smax(_xmax,corners[i].x);

		_ymin=smin(_ymin,corners[i].z);
		_ymax=smax(_ymax,corners[i].z);
	}

	_visBounds.x1=smax(((int)floorf(_xmin))-1,0);
	_visBounds.y1=smax(((int)floorf(_ymin))-1,0);

	_visBounds.x2=smin(((int)ceilf(_xmax))+1,w-1);
	_visBounds.y2=smin(((int)ceilf(_ymax))+1,h-1);
}

void Renderer::addRenderable(Renderable* r)
{
	if(r->isAlwaysVisible())
		_visAlways->addRenderable(r);
	else
		_vis->addRenderable(r);
}

void Renderer::remRenderable(Renderable* r)
{
	if(r->isAlwaysVisible())
		_visAlways->remRenderable(r);
	else
		_vis->remRenderable(r);
}

void Renderer::renderFlags(unsigned int f, const unsigned int* flagMap, int w) const
{
	_device->resetRenderTargets();
	GroundRenderer::getSingletonRef().renderFlag(f,flagMap,w);
}

void Renderer::onResize(int w, int h)
{
	_GBuffer.onResize(w, h);
	HDAO::getSingletonRef().onResize(w, h);
	HDR::getSingletonRef().onResize(w, h);
}