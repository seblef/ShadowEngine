
#include "stdafx.h"
#include "GeometryExportClassDesc.h"
#include "ActorExportClassDesc.h"
#include "ActorAnimClassDesc.h"

HINSTANCE hInstance;
static GeometryExportClassDesc		GeoDesc;
static ActorExportClassDesc			ActorDesc;
static ActorAnimExportClassDesc		ActorAnimDesc;


BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
         hInstance = hinstDLL;
         DisableThreadLibraryCalls(hInstance);
			break;
		}
	return(TRUE);
	}


//------------------------------------------------------
// This is the interface to Jaguar:
//------------------------------------------------------

extern "C" __declspec( dllexport ) const TCHAR*	LibDescription()				{ return _T("Shadow export tools"); }
extern "C" __declspec( dllexport ) int				LibNumberClasses()				{ return 3; }
extern "C" __declspec( dllexport ) ClassDesc*		LibClassDesc(int i)
{
	switch(i)
	{
	case 0:			return &GeoDesc;			break;
	case 1:			return &ActorDesc;			break;
	case 2:			return &ActorAnimDesc;		break;
	default:		return 0;
	}
}
extern "C" __declspec( dllexport ) ULONG			LibVersion()					{ return VERSION_3DSMAX; }
extern "C" __declspec( dllexport ) ULONG			CanAutoDefer()					{ return 0; }
