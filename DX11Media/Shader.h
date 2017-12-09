
#ifndef _SHADER_H_
#define _SHADER_H_

#include <MediaCommon.h>
#include <d3d11.h>

class Shader : public IShader
{
protected:

	ID3D11PixelShader*			_pixel;
	ID3D11VertexShader*			_vertex;
	ID3D11DeviceContext*		_context;

	bool						loadPShader(const string& shader, ID3D11Device* device);
	bool						loadVShader(const string& shader, ID3D11Device* device);

public:

	Shader(ID3D11Device *_device, ID3D11DeviceContext* context, const string& shaderName);
	~Shader();

	void						set();
};


#endif