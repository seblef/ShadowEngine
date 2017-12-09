
#ifndef _NAVPATH_H_
#define _NAVPATH_H_

#include "NavCase.h"
#include <Core.h>

using namespace Core;

class NavPath : public TAllocated<NavPath>
{
protected:

	typedef vector<NavCase>		CaseVector;

	CaseVector					_path;

public:

	NavPath()					{}

	void						clearAllocated()			{ _path.clear(); }

	void						addCase(const NavCase& c)	{ _path.push_back(c); }

	int							getPathLength() const		{ return _path.size(); }
	const NavCase&				getPathStep(int n) const	{ return _path[n]; }

	float						getOrientation(int n) const;

	void						outputPath(ofstream& ofs) const;
};

#endif