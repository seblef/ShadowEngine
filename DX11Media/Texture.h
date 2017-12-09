
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <MediaCommon.h>
#include <D3D11.h>

class Texture : public ITexture
{
protected:

	ID3D11Texture2D*			_texture;
	ID3D11ShaderResourceView*	_shaderView;
	ID3D11RenderTargetView*		_renderTargetView;
	ID3D11DeviceContext*		_context;

	void						load(ID3D11Device* device);
	void						createRenderTarget(ID3D11Device* device);
	void						createEmpty(ID3D11Device* device);

public:

	Texture(ID3D11Device *pDevice, ID3D11DeviceContext* context, const string& name, int width, int height, TextureFormat format, BufferUsage bu,
		bool renderTarget, bool cubeTexture);
	~Texture();

	void						set(int stage, ShaderType=ST_PIXEL);
	bool						lock(void **ppData, int& rowPitch, int& depthPitch);
	void						fill(const void* data);
	void						unlock();
	bool						save(const string& fileName);
	bool						generateMipMaps();

	ID3D11RenderTargetView*		getRenderTargetView() const						{ return _renderTargetView; }
};

#endif