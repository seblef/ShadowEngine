
#include "stdafx.h"
#include "R2D_Font.h"

R2D_Font::R2D_Font(R2D_FontCharArea* table, ITexture* tex) : _table(table), _texture(tex), _maxHeight(0), _wrongChar(0)
{
	assert(table && tex);
	for(int i=0;i<256;++i)
		_maxHeight=smax(_maxHeight,table[i].getSize().x);
}

R2D_Font::~R2D_Font()
{
	delete[] _table;
	delete _texture;
}

void R2D_Font::getTextSize(const string& text, Vector2& size) const
{
	size=Vector2::NullVector;
	Vector2 thisLine(0,_maxHeight);
	char c;

	for(int i=0;i<text.length();++i)
	{
		c=text[i];
		const R2D_FontCharArea& a(getCharArea(c));

		if(c=='\n')
		{
			size.y+=thisLine.y;
			size.x=smax(size.x,thisLine.x);

			thisLine.x=0;
		}
		else
			thisLine.x+=a.getSize().x;
	}

	size.y+=thisLine.y;
	size.x=smax(size.x,thisLine.x);
}