
#ifndef _IPATHFINDER_H_
#define _IPATHFINDER_H_

#include "NavPath.h"

class NavMap;

class IPathFinder
{
public:

	IPathFinder()					{}
	virtual ~IPathFinder()			{}

	virtual bool					findPath(int startX, int startY,
											 int targetX, int targetY,
											 NavPath& p, const NavMap& m)=0;
};

#endif