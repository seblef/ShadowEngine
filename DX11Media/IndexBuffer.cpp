
#include "stdafx.h"
#include "IndexBuffer.h"

const size_t g_IdxSize[IDX_COUNT]={
	sizeof(unsigned short),
	sizeof(unsigned int)
	};

IndexBuffer::IndexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, int idxCount, IndexType idxType,
						 BufferUsage bu, const void *data) : IIndexBuffer(idxCount,idxType,bu), _context(context), _buffer(0)
{
	D3D11_BUFFER_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.Usage=(D3D11_USAGE)bu;
	desc.ByteWidth=idxCount * g_IdxSize[idxType];
	desc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	desc.MiscFlags=0;
	desc.StructureByteStride=0;
	desc.CPUAccessFlags=0;
	
	if(bu==BU_IMMUTABLE || bu==BU_DEFAULT)
		desc.CPUAccessFlags=0;
	else if(bu==BU_STAGING)
		desc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	else
		desc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sub_data;
	memset(&sub_data,0,sizeof(sub_data));
	sub_data.pSysMem=data;
	sub_data.SysMemPitch=0;
	sub_data.SysMemSlicePitch=0;

	device->CreateBuffer(&desc,data ? &sub_data : 0,&_buffer);
	_good=(_buffer!=0);
}

void IndexBuffer::set()
{
	_context->IASetIndexBuffer(_buffer,_idxType==IDX_16BITS ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT,0);
}

void IndexBuffer::fill(const void* data, int count, int offset)
{
	int byte_size=count==0 ? (_idxCount * g_IdxSize[_idxType]) : count * g_IdxSize[_idxType];
	int byte_offset=offset * g_IdxSize[_idxType];

	assert(_good && _usage!=BU_IMMUTABLE && (byte_size + byte_offset) < _idxCount * g_IdxSize[_idxType]);

	if(_usage==BU_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE sub;
		if(_context->Map(_buffer,0,D3D11_MAP_WRITE_DISCARD,0,&sub))
		{
			memcpy((void*)(((byte*)sub.pData) + byte_offset),data,byte_size);
			_context->Unmap(_buffer,0);
		}
	}
	else
	{
		D3D11_BOX box;
		box.left=byte_offset;
		box.right=byte_offset + byte_size;
		box.top=box.front=box.bottom=box.back=0;
		_context->UpdateSubresource(_buffer,0,&box,data,0,0);
	}
}
