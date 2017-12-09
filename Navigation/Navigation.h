
#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <Core.h>
#include "NavMap.h"

using namespace Core;

class Navigation : public TSingleton<Navigation>
{
protected:

	NavMap*				_map;

public:

	Navigation()		{}

	void				createMap(int w, int h)
	{
		_map=new NavMap(w,h);
	}

	void				releaseMap()
	{
		delete _map;	_map=0;
	}

	NavMap&				getMap()					{ return *_map; }
	const NavMap&		getMap() const				{ return *_map; }

	bool				findPath(int startX, int startY,
								 int targetX, int targetY,
								 NavPath& p)
	{
		return _map->getPathFinder()->findPath(startX,startY,targetX,targetY,p,*_map);
	}
};

#endif