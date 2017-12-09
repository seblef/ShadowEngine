
#ifndef _R2D_TEXTRENDERER_H_
#define _R2D_TEXTRENDERER_H_

#include "R2D_Renderer.h"
#include "R2D_Text.h"
#include <algorithm>

class R2D_TextRenderer : public R2D_Renderer
{
protected:

	Vector2					_screenSize;

	static bool				txtSort(const R2D_Object* o1, const R2D_Object* o2)
	{
		return ((const R2D_Text*)o1)->getFont() < ((const R2D_Text*)o2)->getFont();
	}

	void					sortObjects()
	{
		sort(_objects.begin(),_objects.end(),txtSort);
	}
	void					renderObjects();
	void					renderText(const R2D_Text& t) const;

public:

	R2D_TextRenderer(bool relativePos, ITexture* white, IVideoDevice* dev) : R2D_Renderer(R2D_Object::R2D_TEXT,relativePos,white,dev)
	{
		_screenSize.x=(float)dev->getResWidth();
		_screenSize.y=(float)dev->getResHeight();

		_scale.x=2.0f / _screenSize.x;
		_scale.y=2.0f / _screenSize.y;
	}
};

#endif