
#ifndef _GBUFFER_H_
#define _GBUFFER_H_

#include <MediaCommon.h>

class GBuffer
{
protected:

	IVideoDevice*			_device;
	ITexture*				_diffuseRT;
	ITexture*				_normalRT;
	DepthStencilState		_DSState;
	SamplerState			_writeSState;
	SamplerState			_readSState;

	DepthStencilState		_copyDepthState;
	BlendState				_copyBlendState;
	IShader*				_copyShader;
	IConstantBuffer*		_copyCB;

	void					clearResources();
	void					createResources(int w, int h);

	void					copy(const Color& c) const;

public:

	GBuffer(IVideoDevice* device);
	~GBuffer();

	void					setAsRenderTarget() const;
	void					setAsTextures() const;

	void					copyDiffuse() const				{ copy(Color::Red); }
	void					copyNormal() const				{ copy(Color::Green); }
	void					copyDepth() const				{ copy(Color::Blue); }

	void					copyShadowMap(IShadowMap* m) const;
	void					copyTexture(ITexture* m) const;

	void					onResize(int w, int h);
};

#endif