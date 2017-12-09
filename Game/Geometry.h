
#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <Core.h>

using namespace Core;

class Geometry
{
protected:

	int				_vxCount;
	int				_triCount;
	Vx3D*			_vertices;
	unsigned short*	_indices;
	BBox3			_bBox;

public:

	Geometry(int vxCount, int triCount) : _vxCount(vxCount), _triCount(triCount), _bBox(BBox3::BoundBox)
	{
		_vertices=new Vx3D[vxCount];
		_indices=new unsigned short[triCount*3];
	}
	~Geometry()
	{
		delete[] _vertices;
		delete[] _indices;
	}

	int					getVertexCount() const			{ return _vxCount; }
	int					getTriangleCount() const		{ return _triCount; }

	Vx3D*				getVertices() const				{ return _vertices; }
	Vx3D&				getVertex(int n)				{ return _vertices[n]; }
	const Vx3D&			getVertex(int n) const			{ return _vertices[n]; }

	unsigned short*		getTriangle(int n)				{ return _indices + n*3; }
	unsigned short*		getTriangles() const			{ return _indices; }

	void				setBBox(const BBox3& b)			{ _bBox=b; }
	const BBox3&		getBBox() const					{ return _bBox; }

	void				computeBBox();
	void				computeNormals();
	void				computeBinormalAndTangent();

	void				outputVertices() const;
	void				outputTriangles() const;
};

#endif