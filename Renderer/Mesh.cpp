
#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh(int vxCount, const Vx3D* vx, int triCount, const unsigned short* tri,
		   Material* mat, IVideoDevice* device) : _material(mat), _bBox(BBox3::InitBox)
{
	for(int i=0;i<vxCount;++i)
		_bBox << vx[i].pos;

	_vBuffer=device->createVertexBuffer(vxCount,VX_3D,BU_IMMUTABLE,vx);
	_iBuffer=device->createIndexBuffer(triCount*3,IDX_16BITS,BU_IMMUTABLE,tri);
}

Mesh::~Mesh()
{
	delete _vBuffer;
	delete _iBuffer;
}