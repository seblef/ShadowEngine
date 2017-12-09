
#ifndef _SHADOWMAPSBASE_H_
#define _SHADOWMAPSBASE_H_

#include <MediaCommon.h>

class ShadowMapsBase
{
public:

	enum MapSize
	{
		MP_64=0,
		MP_128,
		MP_256,
		MP_512,
		MP_1024,

		MP_COUNT
	};


protected:

	static const int		_mapSizes[MP_COUNT];
	static const int		_mapCounts[MP_COUNT];

	int						_leftMaps[MP_COUNT];
	IShadowMap**			_maps[MP_COUNT];

	MapSize					getBestSize(int size) const
	{
		MapSize ms=MP_1024;
		for(int i=0;i<MP_COUNT;++i)
			if(size <= _mapSizes[i])
			{
				ms=(MapSize)i;
				break;
			}
		return ms;
	}

public:

	ShadowMapsBase(IVideoDevice *device);
	~ShadowMapsBase();

	void					clear()
	{
		for(int i=0;i<MP_COUNT;++i)
			_leftMaps[i]=_mapCounts[i];
	}

	IShadowMap*				getShadowMap(int size);
	IShadowMap*				getShadowMap(MapSize size, int n) const				{ return _maps[size][n]; }
};

#endif