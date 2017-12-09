
#ifndef _NAVCASE_H_
#define _NAVCASE_H_

class NavCase
{
protected:

	int				_x, _y;

public:

	NavCase()												{}
	NavCase(int x, int y) : _x(x), _y(y)					{}
	NavCase(const NavCase& n) : _x(n._x), _y(n._y)			{}
	NavCase(float x, float y) : _x((int)x), _y((int)y)		{}
	
	NavCase&		operator=(const NavCase& c)
	{
		_x=c._x;	_y=c._y;
		return *this;
	}

	int				getX() const							{ return _x; }
	int				getY() const							{ return _y; }

	void			setX(int x)								{ _x=x; }
	void			setY(int y)								{ _y=y; }
};

#endif