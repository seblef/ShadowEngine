
#include "stdafx.h"
#include "ShadowMapsBase.h"

const int ShadowMapsBase::_mapSizes[ShadowMapsBase::MP_COUNT]={
	64,128,256,512,1024
};

const int ShadowMapsBase::_mapCounts[ShadowMapsBase::MP_COUNT]={
	16,16,16,8,4
};

ShadowMapsBase::ShadowMapsBase(IVideoDevice* device)
{
	for(int i=0;i<MP_COUNT;++i)
	{
		_maps[i]=new IShadowMap*[_mapCounts[i]];
		for(int j=0;j<_mapCounts[i];++j)
			_maps[i][j]=device->createShadowMap(_mapSizes[i],_mapSizes[i]);
	}

	clear();
}

ShadowMapsBase::~ShadowMapsBase()
{
	for(int i=0;i<MP_COUNT;++i)
	{
		for(int j=0;j<_mapCounts[i];++j)
			delete _maps[i][j];

		delete[] _maps[i];
	}
}

IShadowMap *ShadowMapsBase::getShadowMap(int size)
{
	MapSize ms=getBestSize(size);
	IShadowMap* m=0;
	if(_leftMaps[ms])
	{
		--_leftMaps[ms];
		m=_maps[ms][_leftMaps[ms]];
	}
	return m;
}