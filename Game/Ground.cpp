
#include "stdafx.h"
#include "Ground.h"
#include <NavigationLib.h>

Ground::Ground(int w, int h, GameMaterial *nullMat) : _width(w), _height(h)
{
	_ground=new unsigned int[w*h];
	memset(_ground,0,sizeof(unsigned int)*w*h);
	_gameMaterials.push_back(nullMat);

	Navigation::getSingletonRef().createMap(w,h);
}

Ground::~Ground()
{
	delete[] _ground;
}

unsigned int Ground::getMaterialID(Material* m)
{
	for(unsigned int i=0;i<_materials.size();++i)
		if(_materials[i]==m)
			return i;

	_materials.push_back(m);
	return _materials.size();
}

void Ground::paintRect(int x, int y, int w, int h, Material* m)
{
	unsigned int id=getMaterialID(m);
	for(int _x=x;_x<x+w;++_x)
		for(int _y=y;_y<y+h;++_y)
			_ground[_x + _y*_width]=id;
}

void Ground::finalize() const
{
	int *g=new int[_width*_height];
	Material **m=new Material*[_materials.size()];

	for(int i=0;i<_materials.size();++i)
		m[i]=_materials[i];

	NavMap& nm(Navigation::getSingletonRef().getMap());

	for(int i=0;i<_width*_height;++i)
	{
		g[i]=((int)(_ground[i] & GMATERIAL_MASK))-1;

		if(_ground[i] & GFLAG_WAKABLE)
			nm.setFlag(i,NAV_WAKABLE);
	}

	GroundData *data=new GroundData(_width,_height,_materials.size(),m,g);
	Renderer::getSingletonRef().setGroundData(data);

	delete[] m;
	delete[] g;
}

void Ground::setFlag(unsigned int f, const SRect& r)
{
	unsigned int *g;
	for(int y=r.y1;y<=r.y2;++y)
	{
		g=_ground+y*_width + r.x1;
		for(int x=r.x1;x<=r.x2;++x,++g)
			(*g)|=f;
	}
}

void Ground::unsetFlag(unsigned int f, const SRect& r)
{
	f=~f;
	unsigned int *g;
	for(int y=r.y1;y<=r.y2;++y)
	{
		g=_ground+y*_width + r.x1;
		for(int x=r.x1;x<=r.x2;++x,++g)
			(*g)&=f;
	}
}