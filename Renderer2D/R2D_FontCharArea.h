
#ifndef _R2D_FONTCHARAREA_H_
#define _R2D_FONTCHARAREA_H_

#include <Core.h>

using namespace Core;

class R2D_FontCharArea
{
protected:

	Vector2			_pos;
	Vector2			_size;
	BBox2			_uvCoords;

public:

	R2D_FontCharArea()				{}
	R2D_FontCharArea(const Vector2& pos, const Vector2& size, const BBox2& uv) : _pos(pos), _size(size), _uvCoords(uv)		{}
	R2D_FontCharArea(const R2D_FontCharArea& a) : _pos(a._pos), _size(a._size), _uvCoords(a._uvCoords)	{}

	const Vector2&	getPosition() const					{ return _pos; }
	const Vector2&	getSize() const						{ return _size; }

	void			setPosition(const Vector2& p)		{ _pos=p; }
	void			setSize(const Vector2& s)			{ _size=s; }

	void			setUVCoords(const BBox2& uv)		{ _uvCoords=uv; }
	const BBox2&	getUVCoords() const					{ return _uvCoords; }
};


#endif