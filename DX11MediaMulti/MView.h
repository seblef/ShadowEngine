
#ifndef _MVIEW_H_
#define _MVIEW_H_

#include <MediaCommon.h>
#include <d3d11.h>

class MView : public IView
{
protected:

	ID3D11Device				*_device;
	ID3D11DeviceContext			*_context;
	IDXGISwapChain				*_swapChain;
	ID3D11RenderTargetView		*_backBufferView;
	ID3D11Texture2D				*_depthBuffer;
	ID3D11DepthStencilView		*_depthView;
	ID3D11ShaderResourceView	*_depthAsTexture;
	ID3D11DepthStencilView		*_depthReadOnly;

	void						releaseResources();
	void						createResources();

public:


	MView(IDXGIFactory1 *factory, ID3D11Device* device, ID3D11DeviceContext *context, void *hwnd, int w, int h);
	~MView();

	void						set();
	void						setDepthBufferAsTexture(int slot);
	void						resize(int w, int h);
	void						present()								{ _swapChain->Present(0,0); }

	ID3D11DepthStencilView*		getDepthView() const					{ return _depthView; }
};

#endif