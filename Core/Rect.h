
#ifndef _SRECT_H_
#define _SRECT_H_

#include <ostream>

namespace Core
{

class SRect
{
public:

	int				x1,x2,y1,y2;

	SRect()						{}
	SRect(int _x1,int _y1, int _x2, int _y2) : x1(_x1), x2(_x2), y1(_y1), y2(_y2)			{}
	SRect(const SRect& r) : x1(r.x1), x2(r.x2), y1(r.y1), y2(r.y2)							{}

	SRect&			operator=(const SRect& r)
	{
		x1=r.x1; x2=r.x2;
		y1=r.y1; y2=r.y2;
		return *this;
	}

	bool			operator==(const SRect& r) const				{ return x1==r.x1 && x2==r.x2 && y1==r.y1 && y2==r.y2; }
	bool			operator!=(const SRect& r) const				{ return !(r==*this); }
};

inline ostream& operator << (ostream& os, const SRect& b)
{
	os << "( " << b.x1 << "," << b.y1 << " )-( " << b.x2 << "," << b.y2 << " )";
	return os;
}


};

#endif