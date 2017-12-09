
#include "stdafx.h"
#include "ActorExport.h"
#include "ActorExporter.h"


int ActorExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	ActorExporter ex;
	ex.doExport(name);

	return 1;
}
