
#ifndef _R2D_OBJECT_H_
#define _R2D_OBJECT_H_

#include <MediaCommon.h>

class R2D_Object
{
	public:

		enum Type
		{
			R2D_RECT=0,
			R2D_IMG=1,
			R2D_TEXT,
			R2D_LINE,

			R2D_COUNT
		};

	private:

		Type					_type;

	protected:

		Vector2					_position;
		Vector2					_size;
		Color					_color;

	public:
	
		R2D_Object(Type type) : _type(type)						{}
		R2D_Object(Type type, const Vector2& pos, const Vector2& size,
			const Color& col) : _type(type), _position(pos), _size(size), _color(col)	{}
		virtual ~R2D_Object()															{}

		Type					getType() const					{ return _type; }

		const Vector2&			getPosition() const				{ return _position; }
		void					setPosition(const Vector2& pos) { _position=pos; }

		const Vector2&			getSize() const					{ return _size; }
		void					setSize(const Vector2& vSize)	{ _size=vSize; }

		const Color&			getColor() const				{ return _color; }
		void					setColor(const Color& cCol)		{ _color=cCol; }
};

#endif