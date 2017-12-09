
#ifndef _R2D_TEXT_H_
#define _R2D_TEXT_H_

#include "R2D_Object.h"
#include "R2D_Font.h"

class R2D_Text : public R2D_Object
{
	protected:

		R2D_Font*				_font;
		string				_text;

	public:

		R2D_Text(R2D_Font* font) : R2D_Object(R2D_TEXT), _font(font)				{}
		R2D_Text(R2D_Font* font, const string& txt) : R2D_Object(R2D_TEXT), _font(font), _text(txt)		{}
		R2D_Text(R2D_Font* font, const string& txt, const Vector2& pos, const Vector2& size, const Color& c) : 
			R2D_Object(R2D_TEXT,pos,size,c), _font(font), _text(txt)			{}

		R2D_Font*				getFont() const							{ return _font; }

		void					setText(const string& txt)				{ _text=txt; }
		string&					getText()								{ return _text; }
		const string&			getText() const							{ return _text; }

};

#endif