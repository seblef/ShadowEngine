
#ifndef _G_BASETOOLHEIGHT_H_
#define _G_BASETOOLHEIGHT_H_

#include "G_BaseToolNull.h"
#include "G_Object.h"


class G_BaseToolHeight : public G_BaseToolNull
{
protected:

	Plane				_plane;

	typedef vector<float>			FloatVector;
	FloatVector			_originals;
	float				_lastHeight;
	float				_translation;

public:

	G_BaseToolHeight()		{}

	virtual void		mouseDown(int x, int y, unsigned short flags);
	virtual void		mouseMove(int x, int y);
	virtual void		mouseUp(int x, int y)							{ _originals.clear(); }

	virtual void		cancel();
};

#endif