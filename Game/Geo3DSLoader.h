
#ifndef _GEO3DSLOADER_H_
#define _GEO3DSLOADER_H_

#include "IGeometryLoader.h"
#include <3dsftk.h>

class Geo3DSLoader : public IGeometryLoader
{
protected:

	Geometry*						loadMesh(database3ds *db);

public:

	Geo3DSLoader()					{}
	~Geo3DSLoader()					{}

	Geometry*						loadGeometry(const string& geoFileName);
};

#endif