
#ifndef _MESH_H_
#define _MESH_H_

#include "Material.h"
#include <MediaCommon.h>

class Mesh
{
protected:

	Material*			_material;
	IIndexBuffer*		_iBuffer;
	IVertexBuffer*		_vBuffer;
	BBox3				_bBox;

public:

	Mesh(int vxCount, const Vx3D* vx, int triCount, const unsigned short *tri,
		Material* mat, IVideoDevice* device);
	virtual ~Mesh();

	void				setMaterial(Material* m)		{ _material=m; }

	Material*			getMaterial() const				{ return _material; }
	IVertexBuffer*		getVertexBuffer() const			{ return _vBuffer; }
	IIndexBuffer*		getIndexBuffer() const			{ return _iBuffer; }
	const BBox3&		getBBox() const					{ return _bBox; }
};

#endif