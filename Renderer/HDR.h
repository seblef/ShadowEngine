
#ifndef _HDR_H_
#define _HDR_H_

#include <MediaCommon.h>

#define LUMINANCE_COUNT				6

class HDR : public TSingleton<HDR>
{
public:

	enum HDRShaders
	{
		HDRS_FIRSTGREYDS = 0,
		HDRS_GREYDS,
		HDRS_BRIGHT,
		HDRS_BRIGHTDS,
		HDRS_HBLOOM,
		HDRS_VBLOOM,
		HDRS_FINAL,

		HDRS_COUNT
	};

	enum HDRTextures
	{
		HDRT_BRIGHT = 0,
		HDRT_BRIGHTDS,
		HDRT_HBLOOM,
		HDRT_VBLOOM,

		HDRT_COUNT
	};


protected:

	struct HDRBuffer
	{
		Vector2		_invBloomTex;
		Vector2		_greyScaleUV;
		float		_brightThreshold;
		float		_exposure;
		float		_gaussianScalar;
		float		f3BrightPack;
	};

	bool				_enable;

	IVideoDevice*		_device;
	ITexture*			_accumulationBuffer;
	ITexture*			_HDRBackBuffer;
	ITexture*			_luminanceTex[LUMINANCE_COUNT];
	ITexture*			_postTex[HDRT_COUNT];
	IShader*			_shaders[HDRS_COUNT];
	IConstantBuffer*	_cBuffer;
	SamplerState		_pointSampler;
	SamplerState		_linearSampler;
	DepthStencilState	_depthState;
	BlendState			_blendState;

	HDRBuffer			_hdrBuffer;

public:

	HDR(IVideoDevice* d, const Config& cfg);
	~HDR();

	ITexture*			getAccumulationBuffer()	const				{ return _accumulationBuffer; }

	void				setBackBuffer();
	void				process();

	void				onResize(int w, int h);
};

#endif