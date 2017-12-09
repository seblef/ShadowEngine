
#ifndef _AS_CELLITEM_H_
#define _AS_CELLITEM_H_

class AS_CellItem
{
protected:

	int					_which;
	int					_parentX, _parentY;
	int					_GCost;

public:

	AS_CellItem()		{}

	void				setWhich(int w)			{ _which=w; }
	void				setParentX(int x)		{ _parentX=x; }
	void				setParentY(int y)		{ _parentY=y; }
	void				setGCost(int c)			{ _GCost=c; }

	int					getWhich() const		{ return _which; }
	int					getParentX() const		{ return _parentX; }
	int					getParentY() const		{ return _parentY; }
	int					getGCost() const		{ return _GCost; }
};

#endif