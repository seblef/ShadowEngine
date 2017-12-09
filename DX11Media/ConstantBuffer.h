
#ifndef _CONSTANTBUFFER_H_
#define _CONSTANTBUFFER_H_

#include <MediaCommon.h>
#include <D3D11.h>

class ConstantBuffer : public IConstantBuffer
{
protected:

	ID3D11Buffer*			_buffer;
	ID3D11DeviceContext*	_context;

public:

	ConstantBuffer(ID3D11Device *device, ID3D11DeviceContext* context, int size, int slot);
	~ConstantBuffer()												{ if(_buffer)		_buffer->Release(); }

	void					set(ShaderType st);
	void					fill(const void* data)					{ _context->UpdateSubresource(_buffer,0,0,data,0,0); }

};

#endif