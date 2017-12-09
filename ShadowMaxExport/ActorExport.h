
#ifndef _ACTOREXPORT_H_
#define _ACTOREXPORT_H_

#include <max.h>

class ActorExport : public SceneExport
{
public:

	ActorExport()							{}
	~ActorExport()							{}

	int				ExtCount()					{ return 1; }
	const TCHAR *	Ext(int n)					{ return _T("act"); }
	const TCHAR *	LongDesc()					{ return _T("Shadow actor export tool"); }
	const TCHAR *	ShortDesc()					{ return _T("ShadowAct export"); }
	const TCHAR *	AuthorName()				{ return _T("Koyot"); }
	const TCHAR *	CopyrightMessage()			{ return _T(""); }
	const TCHAR *	OtherMessage1()				{ return _T(""); }
	const TCHAR *	OtherMessage2()				{ return _T(""); }
	unsigned int	Version()					{ return 100; }
	void			ShowAbout(HWND hWnd)		{}
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);	// Export file

};

#endif