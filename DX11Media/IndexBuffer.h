
#ifndef _INDEXBUFFER_H_
#define _INDEXBUFFER_H_

#include <MediaCommon.h>
#include <D3D11.h>

class IndexBuffer : public IIndexBuffer
{
protected:

	ID3D11DeviceContext*			_context;
	ID3D11Buffer*					_buffer;

public:

	IndexBuffer(ID3D11Device *devive, ID3D11DeviceContext* context, int idxCount, IndexType idxType,
		BufferUsage bu, const void *data);
	~IndexBuffer()							{ if(_buffer)		_buffer->Release(); }

	void							set();
	void							fill(const void* data, int count=0, int offset=0);
};

#endif