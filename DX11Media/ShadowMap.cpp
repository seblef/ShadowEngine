
#include "stdafx.h"
#include "ShadowMap.h"

ShadowMap::ShadowMap(ID3D11Device* device, ID3D11DeviceContext* context, int w, int h) : IShadowMap(w,h),
	_context(context), _texture(0), _shaderView(0), _depthView(0)
{
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.Width=w;
	desc.Height=h;
	desc.MipLevels=1;
	desc.ArraySize=1;
	desc.Format=DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count=1;
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.BindFlags=D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	device->CreateTexture2D(&desc,0,&_texture);

	D3D11_DEPTH_STENCIL_VIEW_DESC rt_desc={
		rt_desc.Format=DXGI_FORMAT_D32_FLOAT,
		rt_desc.ViewDimension=D3D11_DSV_DIMENSION_TEXTURE2D,
		0
	};

	device->CreateDepthStencilView(_texture,&rt_desc,&_depthView);

	D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
	sr_desc.Format=DXGI_FORMAT_R32_FLOAT;
	sr_desc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
	sr_desc.Texture2D.MostDetailedMip=0;
	sr_desc.Texture2D.MipLevels=1;

	device->CreateShaderResourceView(_texture,&sr_desc,&_shaderView);
	_good=true;
}

ShadowMap::~ShadowMap()
{
	_shaderView->Release();
	_depthView->Release();
	_texture->Release();
}

void ShadowMap::setAsRenderTarget()
{
	ID3D11RenderTargetView* null_view=0;
	_context->OMSetRenderTargets(1,&null_view,_depthView);

	D3D11_VIEWPORT vp;
	vp.Width=(float)_width;
	vp.Height=(float)_height;
	vp.TopLeftX=0;
	vp.TopLeftY=0;
	vp.MinDepth=0;
	vp.MaxDepth=1;
	_context->RSSetViewports(1,&vp);
}