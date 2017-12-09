
#ifndef _SHADOWMAP_H_
#define _SHADOWMAP_H_

#include <MediaCommon.h>
#include <D3D11.h>

class ShadowMap : public IShadowMap
{
protected:

	ID3D11Texture2D*			_texture;
	ID3D11ShaderResourceView*	_shaderView;
	ID3D11DepthStencilView*		_depthView;
	ID3D11DeviceContext*		_context;
		
public:

	ShadowMap(ID3D11Device *pDevice, ID3D11DeviceContext* context, int width, int height);
	~ShadowMap();

	void						set(int stage)					{ _context->PSSetShaderResources(stage,1,&_shaderView); }
	void						setAsRenderTarget();
};


#endif