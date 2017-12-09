
#ifndef _AS_PATHFINDER_H_
#define _AS_PATHFINDER_H_

#include "AS_CellItem.h"
#include "IPathFinder.h"

class AS_PathFinder : public IPathFinder
{
protected:

	int					_width,_height;
	int					_onClosedList;
	AS_CellItem*		_cells;

	int*				_openList;
	int*				_openX;
	int*				_openY;
	int*				_FCost;
	int*				_HCost;

	AS_CellItem&		getCell(int x, int y)			{ return _cells[x+y*_width]; }

public:

	AS_PathFinder(int w, int h);
	~AS_PathFinder();

	bool				findPath(int startX, int startY,
								 int targetX, int targetY,
								 NavPath& p, const NavMap& m);
};

#endif