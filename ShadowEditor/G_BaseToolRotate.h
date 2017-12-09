
#ifndef _G_BASETOOLROTATE_H_
#define _G_BASETOOLROTATE_H_

#include "G_BaseToolNull.h"
#include "G_Object.h"


class G_BaseToolRotate : public G_BaseToolNull
{
protected:

	unsigned short		_flags;

	typedef vector<Vector3>			Vec3Vector;
	Vec3Vector			_originals;

	int					_lastX,_lastY;

	Vector3				_rotation;
	void				snap(G_Object *o) const;

public:

	G_BaseToolRotate()		{}

	virtual void		mouseDown(int x, int y, unsigned short flags);
	virtual void		mouseMove(int x, int y);
	virtual void		mouseUp(int x, int y)										{ _originals.clear(); }

	virtual void		cancel();
};

#endif