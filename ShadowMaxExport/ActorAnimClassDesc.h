
#ifndef _ACTORANIMEXPORTDESC_H_
#define _ACTORANIMEXPORTDESC_H_

#include "ActorAnimExport.h"

class ActorAnimExportClassDesc : public ClassDesc
{
public:

	int 			IsPublic()								{ return 1; }
	void *			Create(BOOL loading = FALSE)			{ return new ActorAnimExport; }
	const TCHAR *	ClassName()								{ return _T("Shadow tools"); }
	SClass_ID		SuperClassID()							{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()								{ return Class_ID(0x34a03f21, 0x5f4e1729); }
	const TCHAR* 	Category()								{ return _T("Actor animation export");  }

};


#endif