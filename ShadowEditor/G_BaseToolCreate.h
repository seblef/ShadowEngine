
#ifndef _G_BASETOOLCREATE_H_
#define _G_BASETOOLCREATE_H_

#include "G_BaseToolTranslate.h"
#include "G_Object.h"

class G_BaseToolCreate : public G_BaseToolTranslate
{
protected:

	G_Object*				_object;
	BBox3					_box;

public:

	G_BaseToolCreate() : _object(0)			{}

	void					setObject(G_Object* o, const BBox3& b)				{ _object=o; _box=b; }

	void					mouseDown(int x, int y, unsigned short flags);
	void					mouseUp(int x, int y);

	void					cancel();
};

#endif