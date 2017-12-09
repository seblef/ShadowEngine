
#include "stdafx.h"
#include "Shader.h"
#include <D3DX11.h>


extern ofstream			g_MediaOut;


Shader::Shader(ID3D11Device* _device, ID3D11DeviceContext* context, const string& shaderName) : IShader(shaderName),
	_pixel(0), _vertex(0), _context(context)
{
	_good=loadPShader(shaderName,_device) && loadVShader(shaderName,_device);
}

Shader::~Shader()
{
	if(_pixel)				_pixel->Release();
	if(_vertex)				_vertex->Release();
}

bool Shader::loadPShader(const string& shader, ID3D11Device* device)
{
	ID3D10Blob *pCode;
	ID3D10Blob *pErrors;

	string file("Shaders/");
	file+=shader;
	file+="_ps.hlsl";

#ifdef _DEBUG
	const DWORD dwFlags=D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	const DWORD dwFlags=D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	HRESULT hRes=D3DX11CompileFromFile(file.c_str(),0,0,"PSMain","ps_5_0",dwFlags,0,0,&pCode,&pErrors,0);
	if(hRes!=S_OK)
	{
		g_MediaOut << "x [Shader::LoadPShader]: Erreur lors de la compilation du pixel shader " << file << ":" << endl << (const char*)(pErrors ? pErrors->GetBufferPointer() : "") << endl;

		if(pCode) pCode->Release();
		if(pErrors) pErrors->Release();
		return false;
	}

	hRes=device->CreatePixelShader(pCode->GetBufferPointer(),pCode->GetBufferSize(),0,&_pixel);

	if(pCode)	pCode->Release();
	if(pErrors)	pErrors->Release();

	if(hRes==S_OK)
		return true;
	else
	{
		g_MediaOut << "x [Shader::LoadPShader] <" << shader << ">: Can't create pixel shader." << endl;
		return false;
	}
}

bool Shader::loadVShader(const string& shader, ID3D11Device *device)
{
	ID3D10Blob *pCode;
	ID3D10Blob *pErrors;

	string file("Shaders/");
	file+=shader;
	file+="_vs.hlsl";

#ifdef _DEBUG
	const DWORD dwFlags=D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	const DWORD dwFlags=D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	HRESULT hRes=D3DX11CompileFromFile(file.c_str(),0,0,"VSMain","vs_5_0",dwFlags,0,0,&pCode,&pErrors,0);
	if(hRes!=S_OK)
	{
		g_MediaOut << "x [Shader::LoadVShader]: Erreur lors de la compilation du vertex shader " << file << ":" << endl << (const char*)(pErrors ? pErrors->GetBufferPointer() : "") << endl;
		if(pCode) pCode->Release();
		if(pErrors) pErrors->Release();
		return false;
	}

	hRes=device->CreateVertexShader(pCode->GetBufferPointer(),pCode->GetBufferSize(),0,&_vertex);

	if(pCode)	pCode->Release();
	if(pErrors)	pErrors->Release();

	if(hRes==S_OK)
		return true;
	else
	{
		g_MediaOut << "x [Shader::LoadVShader] <" << shader << ">: Can't create vertex shader." << endl;
		return false;
	}
}

void Shader::set()
{
	_context->VSSetShader(_vertex,0,0);
	_context->PSSetShader(_pixel,0,0);
}