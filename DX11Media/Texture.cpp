
#include "stdafx.h"
#include "Texture.h"
#include <D3DX11.h>

extern ofstream			g_MediaOut;

const DXGI_FORMAT g_TexFConversion[TEXF_COUNT]={
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_B8G8R8X8_UNORM,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
};

const int g_TexelBytes[TEXF_COUNT]={
	4,
	4,
	4,
	8,
	4,
	8,
	16
};

Texture::Texture(ID3D11Device *device, ID3D11DeviceContext* context, const string& name, int width, int height, TextureFormat format, BufferUsage bu,
				 bool renderTarget, bool cubeTexture) : ITexture(name,TF_NONE,width,height,format,bu),
				 _texture(0), _shaderView(0), _renderTargetView(0), _context(context)
{
	if(width==0)				load(device);
	else if(renderTarget)		createRenderTarget(device);
	else						createEmpty(device);
}

Texture::~Texture()
{
	if(_shaderView)					_shaderView->Release();
	if(_texture)					_texture->Release();
	if(_renderTargetView)			_renderTargetView->Release();
}

void Texture::load(ID3D11Device* device)
{
	D3DX11_IMAGE_LOAD_INFO info;
	memset(&info,0,sizeof(info));
	info.BindFlags=D3D11_BIND_SHADER_RESOURCE;
	info.Format=DXGI_FORMAT_FROM_FILE;
	info.Usage=(D3D11_USAGE)_usage;

	if(D3DX11CreateShaderResourceViewFromFile(device,_name.c_str(),&info,0,&_shaderView,0)==S_OK)
	{
		for(int i=0;i<TEXF_COUNT;++i)
		{
			if(g_TexFConversion[i]==info.Format)
				_format=(TextureFormat)i;
		}

		D3DX11_IMAGE_INFO img;
		HRESULT hRes;
		D3DX11GetImageInfoFromFile(_name.c_str(),0,&img,&hRes);

		_width=img.Width;
		_height=img.Height;

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		_shaderView->GetDesc(&desc);

		_flags=TF_GOOD;
		if(desc.ViewDimension==D3D11_SRV_DIMENSION_TEXTURECUBE)
			_flags|=TF_CUBETEXTURE;
	}
	else
		g_MediaOut << "x [Texture::Load]: Error loading texture " << _name << endl;
}

void Texture::createRenderTarget(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.Width=_width;
	desc.Height=_height;
	desc.MipLevels=1;
	desc.ArraySize=1;
	desc.Format=g_TexFConversion[_format];
	desc.SampleDesc.Count=1;
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.BindFlags=D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if(device->CreateTexture2D(&desc,0,&_texture)==S_OK)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rt_desc;
		rt_desc.Format=desc.Format;
		rt_desc.ViewDimension=D3D11_RTV_DIMENSION_TEXTURE2D;
		rt_desc.Texture2D.MipSlice=0;
		device->CreateRenderTargetView(_texture,&rt_desc,&_renderTargetView);

		D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
		sr_desc.Format=desc.Format;
		sr_desc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
		sr_desc.Texture2D.MostDetailedMip=0;
		sr_desc.Texture2D.MipLevels=1;
		device->CreateShaderResourceView(_texture,&sr_desc,&_shaderView);

		if(_shaderView && _renderTargetView)
			_flags=TF_GOOD | TF_RENDERTARGET;
	}
}

void Texture::createEmpty(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.Width=_width;
	desc.Height=_height;
	desc.MipLevels=1;
	desc.ArraySize=1;
	desc.Format=g_TexFConversion[_format];
	desc.SampleDesc.Count=1;
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.BindFlags=D3D11_BIND_SHADER_RESOURCE;

	if(device->CreateTexture2D(&desc,0,&_texture)==S_OK)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
		sr_desc.Format=desc.Format;
		sr_desc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
		sr_desc.Texture2D.MostDetailedMip=0;
		sr_desc.Texture2D.MipLevels=1;

		if(device->CreateShaderResourceView(_texture,&sr_desc,&_shaderView)==S_OK)
			_flags=TF_GOOD;
	}
}

void Texture::set(int stage, ShaderType st)
{
	switch(st)
	{
	case ST_VERTEX:		_context->VSSetShaderResources(stage,1,&_shaderView);			break;
	case ST_PIXEL:		_context->PSSetShaderResources(stage,1,&_shaderView);			break;
	}
}

bool Texture::lock(void **ppData, int& rowPitch, int& depthPitch)
{
	assert(_shaderView);

	ID3D11Resource *pRes;
	_shaderView->GetResource(&pRes);
		
	D3D11_MAPPED_SUBRESOURCE sub;
	if(_context->Map(pRes,0,D3D11_MAP_READ,0,&sub))
	{
		*ppData=sub.pData;
		rowPitch=sub.RowPitch;
		depthPitch=sub.DepthPitch;
		pRes->Release();
		return true;
	}
	else
	{
		pRes->Release();
		g_MediaOut << "x [Texture::Lock]: Failed mapping resource " << _name << endl;
		return false;
	}
}

void Texture::unlock()
{
	ID3D11Resource *pRes;
	_shaderView->GetResource(&pRes);
	_context->Unmap(pRes,0);
	pRes->Release();
}

void Texture::fill(const void* data)
{
	assert(_texture);
	D3D11_MAPPED_SUBRESOURCE sub;
	sub.RowPitch=g_TexelBytes[_format] * _width;
	sub.DepthPitch=sub.RowPitch * _height;

	_context->UpdateSubresource(_texture,0,0,data,
		g_TexelBytes[_format] * _width, g_TexelBytes[_format] * _width * _height);
}

bool Texture::save(const string& fileName)
{
	assert(_shaderView);
	ID3D11Resource *pRes;
	if(_texture)			pRes=(ID3D11Resource*)_texture;
	else					_shaderView->GetResource(&pRes);

	if(D3DX11SaveTextureToFile(_context,pRes,D3DX11_IFF_DDS,fileName.c_str())!=S_OK)
	{
		g_MediaOut << "x [Texture::Save]: Failed saving texture " << fileName << endl;
		return false;
	}
	else
		return true;
}

bool Texture::generateMipMaps()
{
	assert(_shaderView);
	ID3D11Resource *pRes;
	if(_texture)			pRes=_texture;
	else 					_shaderView->GetResource(&pRes);

	return D3DX11FilterTexture(_context,pRes,0,D3DX11_FILTER_BOX)==S_OK;
}
