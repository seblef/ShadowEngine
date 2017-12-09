
#ifndef _GEOMETRYLOADER_H_
#define _GEOMETRYLOADER_H_

#include "Geometry.h"

class GeometryLoader
{
public:

	GeometryLoader()					{}

	static Geometry*					loadGeometry(const string& geoFileName);
};

#endif