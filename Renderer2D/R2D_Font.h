
#ifndef _R2D_FONT_H_
#define _R2D_FONT_H_

#include <MediaCommon.h>
#include "R2D_FontCharArea.h"

class R2D_Font
{
	protected:

		R2D_FontCharArea*		_table;
		ITexture*				_texture;
		unsigned int			_wrongChar;
		float					_maxHeight;

	public:

		R2D_Font(R2D_FontCharArea* table, ITexture* tex);
		~R2D_Font();

		const R2D_FontCharArea&	getCharArea(unsigned char c) const			{ return _table[c]; }
		ITexture*				getTexture() const							{ return _texture; }
		float					getMaxHeight() const						{ return _maxHeight; }

		void					getTextSize(const string& text, Vector2& size) const;
		Vector2					getTextSize(const string& text) const
		{
			Vector2 size;
			getTextSize(text,size);
			return size;
		}
};

#endif