
#ifndef _ACTOREXPORTDESC_H_
#define _ACTOREXPORTDESC_H_

#include "ActorExport.h"

class ActorExportClassDesc : public ClassDesc
{
public:

	int 			IsPublic()								{ return 1; }
	void *			Create(BOOL loading = FALSE)			{ return new ActorExport; }
	const TCHAR *	ClassName()								{ return _T("Shadow tools"); }
	SClass_ID		SuperClassID()							{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()								{ return Class_ID(0xc6946ea, 0x2a781e48); }
	const TCHAR* 	Category()								{ return _T("Actor export");  }

};


#endif