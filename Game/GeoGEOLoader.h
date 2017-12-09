
#ifndef _GEOGEOLOADER_H_
#define _GEOGEOLOADER_H_

#include "IGeometryLoader.h"

class GeoGEOLoader : public IGeometryLoader
{
public:

	GeoGEOLoader()					{}
	~GeoGEOLoader()					{}

	Geometry*						loadGeometry(const string& geoFile);
};

#endif