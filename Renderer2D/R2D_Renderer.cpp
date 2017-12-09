
#include "stdafx.h"
#include "R2D_Renderer.h"


R2D_Renderer::R2D_Renderer(R2D_Object::Type type, bool relativePos, ITexture* white, IVideoDevice* device) :
	_whiteTexture(white), _device(device), _scale(2,2)
{
	if(!_relativePosition)
	{
		_scale.x/=(float)device->getResWidth();
		_scale.y/=(float)device->getResHeight();
	}

	int vxCount,indCount;
	unsigned short ind[R2D_MAX_OBJECTS*6];

	if(type==R2D_Object::R2D_LINE)
	{
		_primitiveType=PT_LINELIST;
		vxCount=R2D_MAX_OBJECTS*2;
		indCount=R2D_MAX_OBJECTS*2;

		for(int i=0;i<indCount;++i)
			ind[i]=(unsigned short)i;
	}
	else
	{
		_primitiveType=PT_TRIANGLELIST;
		vxCount=R2D_MAX_OBJECTS*4;
		indCount=R2D_MAX_OBJECTS*6;

		for(int i=0;i<R2D_MAX_OBJECTS;++i)
		{
			ind[i*6]=i*4;
			ind[i*6+1]=i*4+1;
			ind[i*6+2]=i*4+3;
			ind[i*6+3]=i*4+1;
			ind[i*6+4]=i*4+2;
			ind[i*6+5]=i*4+3;
		}
	}

	_verts=new Vx2D[vxCount];
	_vBuffer=device->createVertexBuffer(vxCount,VX_2D,BU_DYNAMIC);
	_iBuffer=device->createIndexBuffer(indCount,IDX_16BITS,BU_IMMUTABLE,ind);
}

R2D_Renderer::~R2D_Renderer()
{
	delete[] _verts;
	delete _vBuffer;
	delete _iBuffer;
}

void R2D_Renderer::execute()
{
	_vBuffer->set();
	_iBuffer->set();
	_device->setPrimitiveType(_primitiveType);

	sortObjects();
	renderObjects();

	_objects.clear();
}