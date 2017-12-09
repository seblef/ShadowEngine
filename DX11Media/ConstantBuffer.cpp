
#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context, int size, int slot) : IConstantBuffer(size,slot),
	_buffer(0), _context(context)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth=size * 4 * sizeof(float);
	desc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags=0;
	desc.MiscFlags=0;
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.StructureByteStride=0;

	device->CreateBuffer(&desc,0,&_buffer);
	_good=(_buffer!=0);
}

void ConstantBuffer::set(ShaderType st)
{
	switch(st)
	{
		case ST_VERTEX:		_context->VSSetConstantBuffers(_slot,1,&_buffer);		break;
		case ST_PIXEL:		_context->PSSetConstantBuffers(_slot,1,&_buffer);		break;
		default:
			assert(0);
	}
}
