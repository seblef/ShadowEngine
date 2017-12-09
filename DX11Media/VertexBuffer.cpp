
#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, int vxCount, VertexType vxType,
		BufferUsage bu, const void *data) : IVertexBuffer(vxCount,vxType,bu), _context(context), _buffer(0)
{
	D3D11_BUFFER_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.Usage=(D3D11_USAGE)bu;
	desc.ByteWidth=vxCount * g_VertexSize[vxType];
	desc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
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

void VertexBuffer::set()
{
	UINT uStride=g_VertexSize[_vxType];
	UINT uOffset=0;
	_context->IASetVertexBuffers(0,1,&_buffer,&uStride,&uOffset);
}

void VertexBuffer::fill(const void* data, int count, int offset)
{
	int byte_size=count==0 ? (_vxCount * g_VertexSize[_vxType]) : count * g_VertexSize[_vxType];
	int byte_offset=offset * g_VertexSize[_vxType];

	assert(_good && _usage!=BU_IMMUTABLE && (byte_size + byte_offset) <= _vxCount * g_VertexSize[_vxType]);

	if(_usage==BU_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE sub;
		if(_context->Map(_buffer,0,D3D11_MAP_WRITE_DISCARD,0,&sub)==S_OK)
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
