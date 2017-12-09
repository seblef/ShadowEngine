
#ifndef _R2D_LINE_H_
#define _R2D_LINE_H_

#include "R2D_Object.h"

class R2D_Line : public R2D_Object
{
	public:

		R2D_Line() : R2D_Object(R2D_LINE) {}
		R2D_Line(const Vector2& pos, const Vector2& size, const Color& c) : R2D_Object(R2D_LINE,pos,size,c)			{}
};


#endif
