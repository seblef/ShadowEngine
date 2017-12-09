
#ifndef _R2D_RECT_H_
#define _R2D_RECT_H_

#include "R2D_Object.h"

class R2D_Rect : public R2D_Object
{
	public:

		R2D_Rect() : R2D_Object(R2D_RECT) {}
		R2D_Rect(const Vector2& pos, const Vector2& size, const Color& c) : R2D_Object(R2D_RECT,pos,size,c)		{}
};

#endif