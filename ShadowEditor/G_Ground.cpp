
#include "stdafx.h"
#include "G_Ground.h"
#include "G_Selection.h"
#include "../Game/Ground.h"


G_Ground::G_Ground(int w, int h, int matCount, Material **mats, int *mat_ids, G_GameMaterial* nullMat) :
GroundData(w, h, matCount, mats, mat_ids), _currentMat(-1), _matCount(matCount)
{
	_gMaterials.push_back(nullMat);

	_flags=new unsigned int[w*h];
	_gameMaterials = new unsigned int[w*h];
	memset(_flags,0,sizeof(unsigned int)*w*h);
	memset(_gameMaterials, 0, sizeof(unsigned int)*w*h);

	Renderer::getSingletonRef().setGroundData(this);
	G_Selection::getSingletonRef().createGroundActor(w,h);
}

G_Ground::~G_Ground()
{
	G_Selection::getSingletonRef().releaseGroundActor();
	delete[] _flags;
	delete[] _gameMaterials;
}

void G_Ground::resize(int w, int h)
{
	G_Selection::getSingletonRef().releaseGroundActor();
	G_Selection::getSingletonRef().createGroundActor(w,h);

	GroundTri *gt=new GroundTri[w*h*2];
	int mw=smin(w,_width);
	int mh=smin(h,_height);

	for(int y=0;y<mh;++y)
		for(int x=0;x<mw;++x)
		{
			gt[(x+y*w)*2]=getTriangle(x,y,0);
			gt[(x+y*w)*2+1]=getTriangle(x,y,1);
		}

	for(int y=mh;y<h;++y)
		for(int x=0;x<w;++x)
		{
			gt[(x+y*w)*2].create(x,y,0,-1);
			gt[(x+y*w)*2].create(x,y,1,-1);

		}

	for(int x=mw;x<w;++x)
		for(int y=0;y<mh;++y)
		{
			gt[(x+y*w)*2].create(x,y,0,-1);
			gt[(x+y*w)*2].create(x,y,1,-1);
		}

	delete[] _triangles;
	_triangles=gt;

	unsigned int *f=new unsigned int[w*h];
	unsigned int *gm = new unsigned int[w*h];
	memset(f,0,sizeof(unsigned int)*w*h);
	memset(gm, 0, sizeof(unsigned int)*w*h);

	for(int y=0;y<mh;++y)
		for (int x = 0; x < mw; ++x)
		{
			f[x+y*w]=_flags[x+y*_width];
			gm[x + y*w] = _gameMaterials[x + y*_width];
		}

	delete[] _flags;
	delete[] _gameMaterials;

	_flags=f;
	_gameMaterials = gm;

	_width=w;
	_height=h;

	Renderer::getSingletonRef().getVisibilitySystem()->onResize(w,h);
}

void G_Ground::beginPaint(Material *m, G_GameMaterial* gm)
{
	if(m==0)
		_currentMat=-1;
	else
	{
		_currentMat=-1;
		for(int i=0;i<_matCount;++i)
			if(m==_mats[i])
				_currentMat=i;

		if(_currentMat==-1)
		{
			_currentMat=_matCount;
			Material **nm=new Material*[_matCount+1];
			for(int i=0;i<_matCount;++i)
				nm[i]=_mats[i];

			nm[_matCount]=m;
			delete[] _mats;
			_mats=nm;
			++_matCount;
		}
	}

	if (gm == 0)
		_currentGameMat=0;
	else
	{
		_currentGameMat = 0;
		for (unsigned int i = 1; i < _gMaterials.size(); ++i)
		{
			if (_gMaterials[i] == gm)
				_currentGameMat = i;
		}

		if (_currentGameMat == 0)
		{
			_currentGameMat = _gMaterials.size();
			_gMaterials.push_back(gm);
		}
	}
}

void G_Ground::onLoad(int mat_count, Material **mats, unsigned int *ids,
	int gmat_count, G_GameMaterial** gmats)
{
	delete[] _mats;
	_mats=mats;
	_matCount=mat_count;

	for (int i = 0; i < gmat_count; ++i)
		_gMaterials.push_back(gmats[i]);

	int idx=0;
	for(int y=0;y<_height;++y)
		for(int x=0;x<_width;++x,++idx)
		{
			_triangles[idx*2].setMatID(((int)(ids[idx] & 0xff))-1);
			_triangles[idx*2+1].setMatID(((int)(ids[idx] & 0xff))-1);

			_gameMaterials[idx] = (ids[idx] & GGAMEMATERIAL_MASK) >> 8;
			_flags[idx]=ids[idx] & 0xffff0000;
		}
}