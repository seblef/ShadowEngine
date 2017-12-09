
#ifndef _VIDEODEVICE_H_
#define _VIDEODEVICE_H_

#include <MediaCommon.h>
#include <D3D11.h>
#include "WinWindow.h"

class VideoDevice : public IVideoDevice
{
protected:

	ID3D11Device				*_device;
	ID3D11DeviceContext			*_context;
	IDXGISwapChain				*_swapChain;
	ID3D11RenderTargetView		*_backBufferView;
	ID3D11Texture2D				*_depthBuffer;
	ID3D11DepthStencilView		*_depthView;
	ID3D11DepthStencilView		*_depthReadOnly;
	ID3D11ShaderResourceView	*_depthAsTexture;
	ID3D11InputLayout			*_inLayouts[VX_COUNT];

	ID3D11DepthStencilState*	_currentDState;

	IVertexBuffer*				_FSVBuf;
	IIndexBuffer*				_FSIBuf;
	ID3D11RasterizerState*		_FSRState;

	ID3D11RenderTargetView		*_renderTargets[MAX_RENDER_TARGETS];

	bool						createDevice(const WinWindow& win, bool windowed);
	void						createDepthViews();
	void						createFullscreenQuad();
	void						createInputLayouts();

public:

	VideoDevice(const WinWindow& win, bool windowed);
	~VideoDevice();

	//			----------------------
	//					Resources
	//			----------------------

	IVertexBuffer*		createVertexBuffer(int vxCount, VertexType vxType, BufferUsage bu, const void* data=0) const;
	IIndexBuffer*		createIndexBuffer(int idxCount, IndexType idxType, BufferUsage bu, const void* data=0) const;
	IShader*			createShader(const string& shaderName) const;
	ITexture*			createTexture(const string& name, int width=0, int height=0, TextureFormat format=TEXF_A8R8G8B8, BufferUsage=BU_DEFAULT,
		bool renderTarget=false, bool cubeTexture=false) const;
	IConstantBuffer*	createConstantBuffer(int size, int slot) const;
	IShadowMap*			createShadowMap(int width, int height) const;

	//			-----------------------
	//					States
	//			-----------------------

	BlendState			createBlendState(bool bBlendEnable, BlendMode SrcBlend, BlendMode DestBlend) const;
	DepthStencilState	createDepthStencilState(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare) const;
	DepthStencilState	createDepthStencilStateEx(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare,
		bool bStencilEnable, unsigned char byReadMask, unsigned char byWriteMask,
		CompareFunc cfFrontFunc, StencilOp soFrontStencilFail, StencilOp soFrontDepthFail, StencilOp soFrontStencilPass,
		CompareFunc cfBackFunc, StencilOp soBackStencilFail, StencilOp soBackDepthFail, StencilOp soBackStencilPass);
	SamplerState		createSamplerState(FilterMode filter,
		AddressMode uAddress=ADDRESS_WRAP, AddressMode vAddress=ADDRESS_WRAP,
		AddressMode wAddress=ADDRESS_WRAP) const;
	RenderState			createRenderState(CullMode cull, FillMode fill=FILL_SOLID) const;

	void				getBlendStateDesc(BlendState bs, bool& enable, BlendMode& src, BlendMode& dest) const;
	void				getRenderStateDesc(RenderState rs, CullMode& c, FillMode& f) const;

	void				destroyBlendState(BlendState state) const;
	void				destroyDepthStencilState(DepthStencilState state) const;
	void				destroySamplerState(SamplerState state) const;
	void				destroyRenderState(RenderState state) const;

	void				setBlendState(BlendState state) const;
	void				setDepthStencilState(DepthStencilState state);
	void				setSamplerState(int iSlot, SamplerState state, ShaderType=ST_PIXEL) const;
	void				setSamplerState(int iStart, int iCount, SamplerState *pStates, ShaderType=ST_PIXEL) const;
	void				setRenderState(RenderState state) const;
	void				setPrimitiveType(PrimitiveType pt) const;

	//			--------------------------
	//					Render targets
	//			--------------------------

	void				resetRenderTargets();
	void				setRenderTarget(ITexture *pTexture, int iRT);
	void				applyRenderTargets();

	//			--------------------------
	//					Rendering
	//			--------------------------

	void				newFrame() const;
	void				clearRenderTargets(const Color& cColor) const;
	void				clearDepthStencil() const;

	void				setInputLayer(VertexType vx) const;
	void				setDepthBufferAsTexture(int iSlot);
	void				setStencilRef(unsigned long dwRef);
	void				setViewport(const Vector2& vUL, const Vector2& vDR);

	void				render(int iCount=0, int iStart=0) const;
	void				renderIndexed(int iIndexCount=0, int iIndexStart=0, int iVertOffset=0) const;
	void				renderIndexedInstanced(int iCountPerInstance, int iInstCount,
		int iStartIdxLocation, int iBaseVertLocation, int iStartInstLocation) const;
	void				renderFullscreenQuad();
};

#endif