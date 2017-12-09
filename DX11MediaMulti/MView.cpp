
#include "stdafx.h"
#include "MView.h"

MView::MView(IDXGIFactory1 *factory, ID3D11Device* device,  ID3D11DeviceContext *context, void *hwnd, int w, int h) : IView(w,h),
	_device(device), _context(context)
{
	DXGI_SWAP_CHAIN_DESC sc_desc;
	memset(&sc_desc,0,sizeof(sc_desc));
	sc_desc.BufferCount=2;
	sc_desc.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	sc_desc.BufferDesc.Width=_width;
	sc_desc.BufferDesc.Height=_height;
	sc_desc.BufferDesc.RefreshRate.Numerator=60;
	sc_desc.BufferDesc.RefreshRate.Denominator=1;
	sc_desc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc_desc.OutputWindow=(HWND)hwnd;
	sc_desc.SampleDesc.Count=1;
	sc_desc.SampleDesc.Quality=0;
	sc_desc.Windowed=TRUE;

	factory->CreateSwapChain(device,&sc_desc,&_swapChain);

	createResources();
}

MView::~MView()
{
	releaseResources();
	_swapChain->Release();
}

void MView::releaseResources()
{
	_depthAsTexture->Release();
	_depthBuffer->Release();
	_depthView->Release();
	_depthReadOnly->Release();

	ULONG rel=_backBufferView->Release();
	while(rel > 0)
		rel=_backBufferView->Release();
}

void MView::createResources()
{
	ID3D11Texture2D *backBuffer=0;
	_swapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&backBuffer);

	_device->CreateRenderTargetView(backBuffer,0,&_backBufferView);
	backBuffer->Release();

	//				[ Depth buffer ]
	D3D11_TEXTURE2D_DESC tdesc;
	memset(&tdesc,0,sizeof(tdesc));
	tdesc.Width=_width;
	tdesc.Height=_height;
	tdesc.MipLevels=1;
	tdesc.ArraySize=1;
//	tdesc.Format=DXGI_FORMAT_R32_TYPELESS;
	tdesc.Format=DXGI_FORMAT_R24G8_TYPELESS;
	tdesc.SampleDesc.Count=1;
	tdesc.SampleDesc.Quality=0;
	tdesc.Usage=D3D11_USAGE_DEFAULT;
	tdesc.BindFlags=D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags=0;
	tdesc.MiscFlags=0;

	_device->CreateTexture2D(&tdesc,0,&_depthBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC ds_desc;
	memset(&ds_desc,0,sizeof(ds_desc));
//	ds_desc.Format=DXGI_FORMAT_D32_FLOAT;
	ds_desc.Format=DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds_desc.ViewDimension=D3D11_DSV_DIMENSION_TEXTURE2D;
	ds_desc.Texture2D.MipSlice=0;

	_device->CreateDepthStencilView(_depthBuffer,&ds_desc,&_depthView);

	ds_desc.Flags=D3D11_DSV_READ_ONLY_DEPTH;
	_device->CreateDepthStencilView(_depthBuffer,&ds_desc,&_depthReadOnly);

	D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
//	sr_desc.Format=DXGI_FORMAT_R32_FLOAT;
	sr_desc.Format=DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	sr_desc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
	sr_desc.Texture2D.MostDetailedMip=0;
	sr_desc.Texture2D.MipLevels=1;

	_device->CreateShaderResourceView(_depthBuffer,&sr_desc,&_depthAsTexture);
}

void MView::resize(int w, int h)
{
	releaseResources();

	_swapChain->ResizeBuffers(2,w,h,DXGI_FORMAT_R8G8B8A8_UNORM,0);

	_width=w;
	_height=h;

	createResources();
	set();
}

void MView::set()
{
	_context->OMSetRenderTargets(1,&_backBufferView,_depthView);

	D3D11_VIEWPORT vp;
	vp.Width=(float)_width;	vp.Height=(float)_height;
	vp.MinDepth=0.0f;		vp.MaxDepth=1.0f;
	vp.TopLeftX=0;			vp.TopLeftY=0;
	_context->RSSetViewports(1,&vp);
}

void MView::setDepthBufferAsTexture(int slot)
{
	ID3D11RenderTargetView *pTargets[8];
	ID3D11DepthStencilView *pDepth;
	_context->OMGetRenderTargets(8,pTargets,&pDepth);
	_context->OMSetRenderTargets(8,pTargets,_depthReadOnly);
	_context->PSSetShaderResources(slot,1,&_depthAsTexture);
}