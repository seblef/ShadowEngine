
#ifndef _GEOMETRYEXPORTDESC_H_
#define _GEOMETRYEXPORTDESC_H_

#include "GeometryExporter.h"

class GeometryExportClassDesc : public ClassDesc
{
public:

	int 			IsPublic()								{ return 1; }
	void *			Create(BOOL loading = FALSE)			{ return new GeometryExport; }
	const TCHAR *	ClassName()								{ return _T("Shadow tools"); }
	SClass_ID		SuperClassID()							{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()								{ return Class_ID(0x416370a0, 0x1ce91bbd); }
	const TCHAR* 	Category()								{ return _T("Mesh export");  }

};


#endif