
#ifndef _ACTORANIMEXPORT_H_
#define _ACTORANIMEXPORT_H_

#include <max.h>

class ActorAnimExport : public SceneExport
{
public:

	ActorAnimExport()							{}
	~ActorAnimExport()							{}

	int				ExtCount()					{ return 1; }
	const TCHAR *	Ext(int n)					{ return _T("aan"); }
	const TCHAR *	LongDesc()					{ return _T("Shadow actor animation export tool"); }
	const TCHAR *	ShortDesc()					{ return _T("ShadowActorAnim export"); }
	const TCHAR *	AuthorName()				{ return _T("Koyot"); }
	const TCHAR *	CopyrightMessage()			{ return _T(""); }
	const TCHAR *	OtherMessage1()				{ return _T(""); }
	const TCHAR *	OtherMessage2()				{ return _T(""); }
	unsigned int	Version()					{ return 100; }
	void			ShowAbout(HWND hWnd)		{}
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);	// Export file

};

#endif