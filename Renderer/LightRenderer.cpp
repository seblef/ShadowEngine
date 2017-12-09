
#include "stdafx.h"
#include "LightRenderer.h"
#include "Renderer.h"
#include <algorithm>

extern ofstream g_MediaOut;

LightRenderer::LightRenderer(const LightRendererCreate_t& c, const string& noShadowShader, const string& shadowShader) : 
	_device(c._device), _cBuffer(c._cLightBuffer), _rsIn(c._rsIn), _rsOut(c._rsOut), _dsIn(c._dsIn), _dsOut(c._dsOut),
	_vBuffer(0), _iBuffer(0), _indicesCount(0), _shadowBuffer(c._shadowBuffer)
{
	_noShadowShader=_device->createShader(noShadowShader);
	_shadowShader=_device->createShader(shadowShader);
}

LightRenderer::~LightRenderer()
{
	delete _vBuffer;
	delete _iBuffer;
	delete _noShadowShader;
	delete _shadowShader;
}

void LightRenderer::applyLights(const Vector3& eyePos)
{
	if(_lights.empty())
		return;

	sort(_lights.begin(),_lights.end(),lightSort);

	_vBuffer->set();
	_iBuffer->set();

	bool shadows;
	LightVector::const_iterator i(_lights.begin());
	while(i!=_lights.end())
	{
		shadows=(*i)->getShadowMap()!=0;
		if(shadows)
			_shadowShader->set();
		else
			_noShadowShader->set();

		while(i!=_lights.end() && ((*i)->getShadowMap()!=0)==shadows)
		{
			renderLight(*i,eyePos);
			++i;
		}
	}

	_lights.clear();
}

void LightRenderer::renderLight(const Light* l, const Vector3& eyePos)
{
	LightBuffer_t lb;
	Matrix4 world;
	Vector3 pos;

	fillBufferAndGetMatrix(lb,l,world);

	Renderer::getSingletonRef().setObjectTransformation(world);
	pos=l->getWorldMatrix();

	lb._color=l->getColor();
	lb._pos=pos;
	lb._range=l->getRange();
	lb._invRange=1.0f / lb._range;
	_cBuffer->fill(&lb);

	if(isCameraInVolume(eyePos,pos,l))
	{
		_device->setDepthStencilState(_dsIn);
		_device->setRenderState(_rsIn);
	}
	else
	{
		_device->setDepthStencilState(_dsOut);
		_device->setRenderState(_rsOut);
	}

	_device->setStencilRef(1);

	if(l->getShadowMap())
	{
		l->getShadowMap()->set(7);
		_shadowBuffer->setupLight(l->getShadowMap(),l->getShadowMapFilter(),l->getShadowViewProjMatrix());
	}

	_device->renderIndexed(_indicesCount);
}