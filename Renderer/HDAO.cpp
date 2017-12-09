
#include "stdafx.h"
#include "HDAO.h"


HDAO::HDAO(IVideoDevice* d, const Config& cfg) : _enable(true), _device(d),
	_shader(0), _HDAOBuffer(0), _blendState(0), _samplerState(0)
{
	int w, h;
	d->getResolution(w, h);

	_hBuffer._rejectRadius = 0.43f;
	_hBuffer._acceptRadius = 0.06f;
	_hBuffer._intensity = 2.14f;
	_hBuffer._normalScale = 0.3f;
	_hBuffer._acceptAngle = 0.94f;
	_hBuffer._renderTargetSize.x = (float)w;
	_hBuffer._renderTargetSize.y = (float)h;

	cfg.getVar("HDAO_enable", _enable);
	cfg.getVar("HDAO_reject_radius", _hBuffer._rejectRadius);
	cfg.getVar("HDAO_accept_radius", _hBuffer._acceptRadius);
	cfg.getVar("HDAO_intensity", _hBuffer._intensity);
	cfg.getVar("HDAO_normal_scale", _hBuffer._normalScale);
	cfg.getVar("HDAO_accept_angle", _hBuffer._acceptAngle);

	if (_enable)
	{
		_shader = d->createShader("PostProcess/HDAO");
		_HDAOTexture = d->createTexture("HDAO", w, h, TEXF_R32F, BU_DEFAULT, true);
		_HDAOBuffer = d->createConstantBuffer(sizeof(HDAOBufffer_t) / (4 * sizeof(float)), 1);

		_blendState = d->createBlendState(false, BLEND_ONE, BLEND_ZERO);
		_samplerState = d->createSamplerState(FILTER_POINT, ADDRESS_CLAMP, ADDRESS_CLAMP);
	}
	else
		_HDAOTexture = d->createTexture("Textures/Default/Diffuse.bmp");
}

HDAO::~HDAO()
{
	if (_shader)			delete _shader;
	if (_HDAOBuffer)		delete _HDAOBuffer;
	if (_HDAOTexture)		delete _HDAOTexture;

	if (_blendState)		_device->destroyBlendState(_blendState);
	if (_samplerState)		_device->destroySamplerState(_samplerState);
}

void HDAO::process(GBuffer& gbuf, Camera& c)
{
	if (!_enable)
		return;

	_shader->set();
	_device->resetRenderTargets();
	_device->setRenderTarget(_HDAOTexture, 0);
	_device->applyRenderTargets();

	_hBuffer._q = c.getZMax() / (c.getZMax() - c.getZMin());
	_hBuffer._qTimesNear = _hBuffer._q * c.getZMin();

	_HDAOBuffer->set(ST_PIXEL);
	_HDAOBuffer->fill(&_hBuffer);

	_device->setBlendState(_blendState);

	SamplerState ss[4] = { _samplerState, _samplerState, _samplerState, _samplerState };
	_device->setSamplerState(0, 4, ss);

	gbuf.setAsTextures();

	_device->renderFullscreenQuad();
}

void HDAO::onResize(int w, int h)
{
	if (_enable)
	{
		delete _HDAOTexture;
		_HDAOTexture = _device->createTexture("HDAO", w, h, TEXF_R32F, BU_DEFAULT, true);

		_hBuffer._renderTargetSize.x = (float)w;
		_hBuffer._renderTargetSize.y = (float)h;
	}
}