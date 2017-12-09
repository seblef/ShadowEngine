
#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include <MediaCommon.h>
#include <D3D11.h>

class VertexBuffer : public IVertexBuffer
{
protected:

	ID3D11DeviceContext*			_context;
	ID3D11Buffer*					_buffer;

public:

	VertexBuffer(ID3D11Device *devive, ID3D11DeviceContext* context, int vxCount, VertexType vxType,
		BufferUsage bu, const void *data);
	~VertexBuffer()							{ if(_buffer)		_buffer->Release(); }

	void							set();
	void							fill(const void* data, int count=0, int offset=0);
};

#endif