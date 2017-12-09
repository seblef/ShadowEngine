
#include "stdafx.h"
#include "MapLoaderFactory.h"
#include "MapTextLoader.h"

IMapLoader* MapLoaderFactory::createMapLoader(const string& name)
{
	return new MapTextLoader;
}