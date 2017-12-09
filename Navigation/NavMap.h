
#ifndef _NAVMAP_H_
#define _NAVMAP_H_

#include "NavDefs.h"
#include "IPathFinder.h"

class NavMap
{
protected:

	int					_width;
	int					_height;
	nByte*				_map;

	IPathFinder*		_pathFinder;

public:

	NavMap(int w, int h);
	~NavMap()
	{
		delete[] _map;
		delete _pathFinder;
	}

	int					getWidth() const						{ return _width; }
	int					getHeight() const						{ return _height; }

	void				setFlag(int n, nByte f)					{ _map[n]|=f; }
	void				unsetFlag(int n, nByte f)				{ _map[n]&= ~f; }

	void				setFlag(int x, int y, nByte f)			{ setFlag(x+y*_width,f); }
	void				unsetFlag(int x, int y, nByte f)		{ unsetFlag(x+y*_width,f); }

	nByte				getFlag(int n, nByte f) const			{ return _map[n] & f; }
	nByte				getFlag(int x, int y, nByte f) const	{ return getFlag(x+y*_width,f); }

	nByte				getMap(int n) const						{ return _map[n]; }
	nByte				getMap(int x, int y) const				{ return _map[x+y*_width]; }

	const nByte*		getMap() const							{ return _map; }

	IPathFinder*		getPathFinder() const					{ return _pathFinder; }

	bool				findBestPlace(int x, int y, float dx, float dy, int& rx, int& ry, bool reverse=false) const;
};

#endif