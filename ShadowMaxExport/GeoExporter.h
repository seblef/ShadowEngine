
#ifndef _GEOEXPORTER_H_
#define _GEOEXPORTER_H_

#include "IExporter.h"
#include "TGeoBuilder.h"

class GeoExporter : public IExporter
{
protected:

	Core::TGeoBuilder<Core::Vx3D>			_builder;

	void									exportNode(IGameNode* node);

public:

	GeoExporter()							{}
	~GeoExporter()							{}

	void									doExport(const string& fileName);
};

#endif