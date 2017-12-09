
#include "stdafx.h"
#include "ActorAnimExport.h"
#include "AnimExporter.h"


int ActorAnimExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	AnimExporter ex;
	ex.doExport(name);

	return 1;
}
