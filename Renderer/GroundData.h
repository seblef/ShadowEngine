
#ifndef _GROUNDDATA_H_
#define _GROUNDDATA_H_

#include "Material.h"
#include "GroundTri.h"

class GroundData
{
protected:

	int					_width,_height;
	GroundTri*			_triangles;
	Material**			_mats;

public:

	GroundData(int width, int height, int mat_count, Material** mats, int *mat_ids);
	~GroundData();

	int					getWidth() const								{ return _width; }
	int					getHeight() const								{ return _height; }
	Material&			getMaterial(int n) const						{ return *_mats[n]; }

	const GroundTri&	getTriangle(int n) const						{ return _triangles[n]; }
	const GroundTri&	getTriangle(int x, int y, int side) const		{ return _triangles[(x+y*_width)*2 + side]; }
	const GroundTri*	getTriangles() const							{ return _triangles; }
};

#endif