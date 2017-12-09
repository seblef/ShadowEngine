
#include "stdafx.h"
#include "GeometryExporter.h"
#include "GeoExporter.h"


int GeometryExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	GeoExporter ex;
	ex.doExport(name);

	return 1;
}
