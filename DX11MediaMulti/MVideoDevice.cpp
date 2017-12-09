
#include "stdafx.h"
#include "MVideoDevice.h"
#include "../DX11Media/VertexBuffer.h"
#include "../DX11Media/IndexBuffer.h"
#include "../DX11Media/Shader.h"
#include "../DX11Media/ShadowMap.h"
#include "../DX11Media/ConstantBuffer.h"
#include "../DX11Media/Texture.h"
#include <D3DX11.h>

extern ofstream			g_MediaOut;


const D3D11_CULL_MODE CullModeConvers[CULL_COUNT]={
	D3D11_CULL_NONE,
	D3D11_CULL_FRONT,
	D3D11_CULL_BACK
};

const D3D11_BLEND BlendModeConvers[BLEND_COUNT]={
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_BLEND_FACTOR
};

const D3D11_COMPARISON_FUNC CompareConvers[COMP_COUNT]={
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};

const D3D11_STENCIL_OP StencilConvers[STENCIL_COUNT]={
	D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_REPLACE,
	D3D11_STENCIL_OP_INCR_SAT,
	D3D11_STENCIL_OP_DECR_SAT,
	D3D11_STENCIL_OP_INVERT,
	D3D11_STENCIL_OP_INCR,
	D3D11_STENCIL_OP_DECR
};

const D3D11_FILL_MODE FillConvers[FILL_COUNT]={
	D3D11_FILL_SOLID,
	D3D11_FILL_WIREFRAME,
	D3D11_FILL_WIREFRAME
};

const D3D11_FILTER FilterConvers[FILTER_COUNT]={
	D3D11_FILTER_MIN_MAG_MIP_POINT,
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MIN_MAG_MIP_LINEAR
};

const D3D11_PRIMITIVE_TOPOLOGY PrimitiveConvers[PT_COUNT]=
{
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST
};

const D3D11_TEXTURE_ADDRESS_MODE AddressConvers[ADDRESS_COUNT]=
{
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_BORDER,
	D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};



MVideoDevice::MVideoDevice(HINSTANCE hInst) : IMultiVideoDevice(0,0,true), _device(0), _context(0),
	_factory(0), _currentView(0),
	_currentDState(0), _FSIBuf(0), _FSVBuf(0), _FSRState(0)
{
	for(int i=0;i<VX_COUNT;++i)
		_inLayouts[i]=0;

	for(int i=0;i<MAX_RENDER_TARGETS;++i)
		_renderTargets[i]=0;

	createDevice(hInst);
	createInputLayouts();
	createFullscreenQuad();
	_good=true;
}

MVideoDevice::~MVideoDevice()
{
	delete _FSVBuf;
	delete _FSIBuf;
	_FSRState->Release();

	for(int i=0;i<VX_COUNT;++i)
		_inLayouts[i]->Release();

	_context->Release();
	_device->Release();
	_factory->Release();
}

void MVideoDevice::resizeView(IView *v, int w, int h)
{
	if(v==_currentView)
	{
		_context->OMSetRenderTargets(0,0,0);
		_context->ClearState();
	}

	((MView*)v)->resize(w,h);

	if(v==_currentView)
		v->set();
}

void MVideoDevice::createDevice(HINSTANCE hInst)
{
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&_factory));

	D3D_FEATURE_LEVEL feat_level=D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL feat_level_sup;
	D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,0,0,
		&feat_level,1, D3D11_SDK_VERSION, &_device,&feat_level_sup,&_context);
}

void MVideoDevice::createFullscreenQuad()
{
	Vx2D v[4]={
		Vx2D(Vector3(-1.0f,-1.0f,0.5f),	Color::White, Vector2(0.0f,1.0f)),
		Vx2D(Vector3( 1.0f,-1.0f,0.5f), Color::White, Vector2(1.0f,1.0f)),
		Vx2D(Vector3( 1.0f, 1.0f,0.5f), Color::White, Vector2(1.0f,0.0f)),
		Vx2D(Vector3(-1.0f, 1.0f,0.5f), Color::White, Vector2(0.0f,0.0f)) };
	unsigned short ind[6]={0, 1, 2, 0, 2, 3};

	_FSVBuf=createVertexBuffer(5,VX_2D,BU_IMMUTABLE,v);
	_FSIBuf=createIndexBuffer(6,IDX_16BITS,BU_IMMUTABLE,ind);

	_FSRState=(ID3D11RasterizerState*)createRenderState(CULL_NONE);
}

IVertexBuffer* MVideoDevice::createVertexBuffer(int vxCount, VertexType vxType, BufferUsage bu, const void* data) const
{
	return new VertexBuffer(_device,_context,vxCount,vxType,bu,data);
}

IIndexBuffer* MVideoDevice::createIndexBuffer(int idxCount, IndexType idxType, BufferUsage bu, const void* data) const
{
	return new IndexBuffer(_device,_context,idxCount,idxType,bu,data);
}

IShader* MVideoDevice::createShader(const string& shaderName) const
{
	return new Shader(_device,_context,shaderName);
}

ITexture* MVideoDevice::createTexture(const string& name, int width, int height, TextureFormat format, BufferUsage bu,
		bool renderTarget, bool cubeTexture) const
{
	return new Texture(_device,_context,name,width,height,format,bu,renderTarget,cubeTexture);
}

IConstantBuffer* MVideoDevice::createConstantBuffer(int size, int slot) const
{
	return new ConstantBuffer(_device,_context,size,slot);
}

IShadowMap*	MVideoDevice::createShadowMap(int width, int height) const
{
	return new ShadowMap(_device,_context,width,height);
}


BlendState MVideoDevice::createBlendState(bool bBlendEnable, BlendMode SrcBlend, BlendMode DestBlend) const
{
	D3D11_BLEND_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.RenderTarget[0].BlendEnable=bBlendEnable ? TRUE : FALSE;
	desc.RenderTarget[0].DestBlend=BlendModeConvers[DestBlend];
	desc.RenderTarget[0].SrcBlend=BlendModeConvers[SrcBlend];
	desc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
	desc.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
	desc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
	
	ID3D11BlendState *pState=0;
	_device->CreateBlendState(&desc,&pState);

	return (void*)pState;
}

DepthStencilState MVideoDevice::createDepthStencilState(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare) const
{
	D3D11_DEPTH_STENCIL_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.DepthEnable=bZEnable ? TRUE : FALSE;
	desc.DepthFunc=CompareConvers[ZCompare];
	desc.StencilEnable=FALSE;
	desc.DepthWriteMask=bZWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	
	ID3D11DepthStencilState *pState=0;
	_device->CreateDepthStencilState(&desc,&pState);

	return (void*)pState;
}

DepthStencilState MVideoDevice::createDepthStencilStateEx(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare,
		bool bStencilEnable, unsigned char byReadMask, unsigned char byWriteMask,
		CompareFunc cfFrontFunc, StencilOp soFrontStencilFail, StencilOp soFrontDepthFail, StencilOp soFrontStencilPass,
		CompareFunc cfBackFunc, StencilOp soBackStencilFail, StencilOp soBackDepthFail, StencilOp soBackStencilPass)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.DepthEnable=bZEnable ? TRUE : FALSE;
	desc.DepthFunc=CompareConvers[ZCompare];
	desc.DepthWriteMask=bZWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable=bStencilEnable ? TRUE : FALSE;
	desc.StencilReadMask=byReadMask;
	desc.StencilWriteMask=byWriteMask;
	desc.FrontFace.StencilFunc=CompareConvers[cfFrontFunc];
	desc.FrontFace.StencilDepthFailOp=StencilConvers[soFrontDepthFail];
	desc.FrontFace.StencilFailOp=StencilConvers[soFrontStencilFail];
	desc.FrontFace.StencilPassOp=StencilConvers[soFrontStencilPass];
	desc.BackFace.StencilFunc=CompareConvers[cfBackFunc];
	desc.BackFace.StencilDepthFailOp=StencilConvers[soBackDepthFail];
	desc.BackFace.StencilFailOp=StencilConvers[soBackStencilFail];
	desc.BackFace.StencilPassOp=StencilConvers[soBackStencilPass];
	
	ID3D11DepthStencilState *pState=0;
	_device->CreateDepthStencilState(&desc,&pState);

	return (void*)pState;
}

SamplerState MVideoDevice::createSamplerState(FilterMode filter,	AddressMode uAddress, AddressMode vAddress,	AddressMode wAddress) const
{
	D3D11_SAMPLER_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.Filter=FilterConvers[filter];
	desc.AddressU=AddressConvers[uAddress];
	desc.AddressV=AddressConvers[vAddress];
	desc.AddressW=AddressConvers[wAddress];
	desc.MinLOD=-FLT_MAX;
	desc.MaxLOD=FLT_MAX;
	desc.MaxAnisotropy=16;
	desc.ComparisonFunc=D3D11_COMPARISON_NEVER;
	desc.BorderColor[0]=desc.BorderColor[1]=desc.BorderColor[2]=desc.BorderColor[3]=0.0f;
	desc.MipLODBias=0.0f;


	ID3D11SamplerState *pState=0;
	_device->CreateSamplerState(&desc,&pState);

	return (void*)pState;
}

RenderState	MVideoDevice::createRenderState(CullMode cull, FillMode fill) const
{
	D3D11_RASTERIZER_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.CullMode=CullModeConvers[cull];
	desc.FillMode=FillConvers[fill];
	desc.DepthClipEnable=TRUE;

	ID3D11RasterizerState *pState=0;
	_device->CreateRasterizerState(&desc,&pState);

	return (void*)pState;
}

void MVideoDevice::getBlendStateDesc(BlendState bs, bool& enable, BlendMode& src, BlendMode& dest) const
{
	D3D11_BLEND_DESC d;
	((ID3D11BlendState*)bs)->GetDesc(&d);

	if(d.RenderTarget[0].BlendEnable)
	{
		enable=true;
		for(int i=0;i<BLEND_COUNT;++i)
		{
			if(d.RenderTarget[0].SrcBlend==BlendModeConvers[i])
				src=(BlendMode)i;

			if(d.RenderTarget[0].DestBlend==BlendModeConvers[i])
				dest=(BlendMode)i;
		}
	}
	else
	{
		enable=false;
		src=BLEND_ONE;
		dest=BLEND_ZERO;
	}
}

void MVideoDevice::getRenderStateDesc(RenderState rs, CullMode& c, FillMode& f) const
{
	D3D11_RASTERIZER_DESC d;
	((ID3D11RasterizerState*)rs)->GetDesc(&d);

	for(int i=0;i<CULL_COUNT;++i)
		if(d.CullMode==CullModeConvers[i])
			c=(CullMode)i;

	for(int i=0;i<FILL_COUNT;++i)
		if(d.FillMode==FillConvers[i])
			f=(FillMode)i;
}

void MVideoDevice::destroyBlendState(BlendState state) const
{
	assert(state);
	((ID3D11BlendState*)state)->Release();
}

void MVideoDevice::destroyDepthStencilState(DepthStencilState state) const
{
	assert(state);
	((ID3D11DepthStencilState*)state)->Release();
}

void MVideoDevice::destroySamplerState(SamplerState state) const
{
	assert(state);
	((ID3D11SamplerState*)state)->Release();
}

void MVideoDevice::destroyRenderState(RenderState state) const
{
	assert(state);
	((ID3D11RasterizerState*)state)->Release();
}

void MVideoDevice::setBlendState(BlendState state) const
{
	static const float fNullFact[4]={0.0f,0.0f,0.0f,0.0f};
	_context->OMSetBlendState((ID3D11BlendState*)state,fNullFact,0xffffffff);
}

void MVideoDevice::setDepthStencilState(DepthStencilState state)
{
	_context->OMSetDepthStencilState((ID3D11DepthStencilState*)state,0);
	_currentDState=(ID3D11DepthStencilState*)state;
}

void MVideoDevice::setSamplerState(int iSlot, SamplerState state, ShaderType st) const
{
	ID3D11SamplerState *pStates[8];

	switch(st)
	{
		case ST_VERTEX:
			_context->VSGetSamplers(0,8,pStates);
			pStates[iSlot]=(ID3D11SamplerState*)state;
			_context->VSSetSamplers(0,8,pStates);
			break;
		case ST_PIXEL:
			_context->PSGetSamplers(0,8,pStates);
			pStates[iSlot]=(ID3D11SamplerState*)state;
			_context->PSSetSamplers(0,8,pStates);
			break;
	};
}

void MVideoDevice::setSamplerState(int iStart, int iCount, SamplerState *pStates, ShaderType st) const
{
	ID3D11SamplerState *pD3DStates[8];
	for(int i=0;i<iCount;++i)
		pD3DStates[i]=(ID3D11SamplerState*)pStates[i];

	switch(st)
	{
		case ST_VERTEX:			_context->VSSetSamplers(iStart,iCount,pD3DStates);		break;
		case ST_PIXEL:			_context->PSSetSamplers(iStart,iCount,pD3DStates);		break;
	};
}

void MVideoDevice::setRenderState(RenderState state) const
{
	assert(state);
	_context->RSSetState((ID3D11RasterizerState*)state);
}
	
void MVideoDevice::resetRenderTargets()
{
	_currentView->set();
}

void MVideoDevice::setRenderTarget(ITexture *pTexture, int iRT)
{
	assert(pTexture && pTexture->isRenderTarget() && iRT < MAX_RENDER_TARGETS);
	_renderTargets[iRT]=((Texture*)pTexture)->getRenderTargetView();

	_rtWidth=pTexture->getWidth();
	_rtHeight=pTexture->getHeight();
}

void MVideoDevice::applyRenderTargets()
{
	int iCount=0;
	while(iCount < MAX_RENDER_TARGETS && _renderTargets[iCount]!=0)
		++iCount;

	if(iCount==0)
		resetRenderTargets();
	else
	{
		_context->OMSetRenderTargets(iCount,_renderTargets,_currentView->getDepthView());
		D3D11_VIEWPORT vp;
		vp.Width=(float)_rtWidth;	vp.Height=(float)_rtHeight;
		vp.MinDepth=0.0f;		vp.MaxDepth=1.0f;
		vp.TopLeftX=0;			vp.TopLeftY=0;
		_context->RSSetViewports(1,&vp);
	}

	for(int i=0;i<MAX_RENDER_TARGETS;++i)
		_renderTargets[i]=0;
}

void MVideoDevice::newFrame() const
{
	_currentView->present();
}

void MVideoDevice::clearRenderTargets(const Color& cColor) const
{
	ID3D11RenderTargetView *pTargets[MAX_RENDER_TARGETS];
	_context->OMGetRenderTargets(4,pTargets,0);

	for(int i=0;i<MAX_RENDER_TARGETS;++i)
	{
		if(pTargets[i])
			_context->ClearRenderTargetView(pTargets[i],(const float*)&cColor);
	}
}

void MVideoDevice::clearDepthStencil() const
{
	ID3D11DepthStencilView *pDepth;
	_context->OMGetRenderTargets(0,0,&pDepth);

	if(pDepth)
		_context->ClearDepthStencilView(pDepth,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0);
}

void MVideoDevice::setInputLayer(VertexType vx) const
{
	_context->IASetInputLayout(_inLayouts[vx]);
}

void MVideoDevice::setDepthBufferAsTexture(int iSlot)
{
	_currentView->setDepthBufferAsTexture(iSlot);
}

void MVideoDevice::setStencilRef(unsigned long dwRef)
{
	_context->OMSetDepthStencilState(_currentDState,dwRef);
}

void MVideoDevice::setViewport(const Vector2& vUL, const Vector2& vDR)
{
	D3D11_VIEWPORT vp;
	vp.Width=vDR.x - vUL.x;	vp.Height=vDR.y - vUL.y;
	vp.MinDepth=0.0f;		vp.MaxDepth=1.0f;
	vp.TopLeftX=vUL.x;			vp.TopLeftY=vUL.y;
	_context->RSSetViewports(1,&vp);
}

void MVideoDevice::setPrimitiveType(PrimitiveType pt) const
{
	_context->IASetPrimitiveTopology(PrimitiveConvers[pt]);
}

void MVideoDevice::render(int iCount, int iStart) const
{
	_context->Draw(iCount,iStart);
}

void MVideoDevice::renderIndexed(int iIndexCount, int iIndexStart, int iVertOffset) const
{
	_context->DrawIndexed(iIndexCount,iIndexStart,iVertOffset);
}

void MVideoDevice::renderIndexedInstanced(int iCountPerInstance, int iInstCount,
										 int iStartIdxLocation, int iBaseVertLocation,
										 int iStartInstLocation) const
{
	_context->DrawIndexedInstanced(iCountPerInstance,iInstCount,
		iStartIdxLocation,iBaseVertLocation,iStartInstLocation);
}

void MVideoDevice::renderFullscreenQuad()
{
	_FSVBuf->set();
	_FSIBuf->set();
	setInputLayer(VX_2D);
	setPrimitiveType(PT_TRIANGLELIST);
	_context->RSSetState(_FSRState);
	_context->DrawIndexed(6,0,0);
}

void MVideoDevice::createInputLayouts()
{
	D3D11_INPUT_ELEMENT_DESC veBase[]={
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	DWORD dwCountBase=ARRAYSIZE(veBase);

	D3D11_INPUT_ELEMENT_DESC ve3D[]={
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	DWORD dwCount3D=ARRAYSIZE(ve3D);

	D3D11_INPUT_ELEMENT_DESC veSkin[]={
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	DWORD dwCountSkin=ARRAYSIZE(veSkin);

	D3D11_INPUT_ELEMENT_DESC ve2D[]={
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	DWORD dwCount2D=ARRAYSIZE(ve2D);

	D3D11_INPUT_ELEMENT_DESC *pDescs[VX_COUNT]={ ve3D, veSkin, ve2D, veBase };
	DWORD dwCounts[VX_COUNT]={ dwCount3D, dwCountSkin, dwCount2D, dwCountBase };

	const char * const szValidShaders[]={
		_T("Shaders/Valid/3D_VS.hlsl"),
		_T("Shaders/Valid/Skin_VS.hlsl"),
		_T("Shaders/Valid/2D_VS.hlsl"),
		_T("Shaders/Valid/Base_VS.hlsl") };

	ID3DBlob *pCode=0;
	ID3DBlob *pErrors=0;
	//		Brute-force method
	for(int i=0;i<VX_COUNT;++i)
	{
		if(D3DX11CompileFromFile(szValidShaders[i],0,0,"VSMain","vs_5_0",D3D10_SHADER_OPTIMIZATION_LEVEL0,0,0,&pCode,&pErrors,0)!=S_OK)
		{
			g_MediaOut << "x [VideoDevice11::CreateInputLayout]: Can't load valid vertex shader" << szValidShaders[i] << endl;
			continue;
		}

		if(_device->CreateInputLayout(pDescs[i],dwCounts[i],pCode->GetBufferPointer(),pCode->GetBufferSize(),&_inLayouts[i])!=S_OK)
			g_MediaOut << "x [VideoDevice11::CreateInputLayouts]: Failed creating input layout " << szValidShaders[i] << endl;
	}
}
