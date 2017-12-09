
#ifndef _BBOXTORECT_H_
#define _BBOXTORECT_H_

#include <Core.h>

using namespace Core;

inline void computeRect(const BBox3& b, SRect& r)
{
	r.x1=(int)(floorf(b.getMin().x));			r.x2=(int)(ceilf(b.getMax().x));
	r.y1=(int)(floorf(b.getMin().z));			r.y2=(int)(ceilf(b.getMax().z));
}

inline void computeRect(const BBox3& b, SRect& r, const Vector2& offset)
{
	r.x1=(int)(floorf(b.getMin().x + offset.x));			r.x2=(int)(ceilf(b.getMax().x + offset.x));
	r.y1=(int)(floorf(b.getMin().z + offset.y));			r.y2=(int)(ceilf(b.getMax().z + offset.y));
}

inline void computeRect(const BBox2& b, SRect& r)
{
	r.x1=(int)(floorf(b.getMin().x));			r.x2=(int)(ceilf(b.getMax().x));
	r.y1=(int)(floorf(b.getMin().y));			r.y2=(int)(ceilf(b.getMax().y));
}

inline void computeRect(const BBox2& b, SRect& r, const Vector2& offset)
{
	r.x1=(int)(floorf(b.getMin().x + offset.x));			r.x2=(int)(ceilf(b.getMax().x + offset.x));
	r.y1=(int)(floorf(b.getMin().y + offset.y));			r.y2=(int)(ceilf(b.getMax().y + offset.y));
}
#endif