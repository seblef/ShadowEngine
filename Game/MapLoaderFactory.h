
#ifndef _MAPLOADERFACTORY_H_
#define _MAPLOADERFACTORY_H_

#include "IMapLoader.h"

class MapLoaderFactory
{
public:

	MapLoaderFactory()				{}

	static IMapLoader*				createMapLoader(const string& loaderName);
};

#endif