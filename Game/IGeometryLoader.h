
#ifndef _IGEOMETRYLOADER_H_
#define _IGEOMETRYLOADER_H_

#include "Geometry.h"
#include <string>

class IGeometryLoader
{
public:

	IGeometryLoader()						{}
	virtual ~IGeometryLoader()				{}

	virtual Geometry*						loadGeometry(const string& geoFileName)=0;
};

#endif