
#include "stdafx.h"
#include "GroundData.h"


GroundData::GroundData(int width, int height, int mat_count, Material** mats, int *mat_id) : _width(width), _height(height)
{
	_mats=new Material*[mat_count];
	_triangles=new GroundTri[width*height*2];

	for(int i=0;i<mat_count;++i)
		_mats[i]=mats[i];

	GroundTri *t=_triangles;
	int* id=mat_id;
	for(int y=0;y<height;++y)
		for(int x=0;x<width;++x)
		{
			t->create(x,y,0,*id);		++t;
			t->create(x,y,1,*id);		++t;
			++id;
		}
}

GroundData::~GroundData()
{
	delete[] _mats;
	delete[] _triangles;
}