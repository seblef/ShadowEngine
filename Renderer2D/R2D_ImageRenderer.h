
#ifndef _R2D_IMAGERENDERER_H_
#define _R2D_IMAGERENDERER_H_

#include "R2D_Renderer.h"
#include "R2D_Image.h"
#include <algorithm>

class R2D_ImageRenderer : public R2D_Renderer
{
protected:

	static bool				imgSort(const R2D_Object* o1, const R2D_Object* o2)
	{
		return ((const R2D_Image*)o1)->getTexture() < ((const R2D_Image*)o2)->getTexture();
	}

	void					sortObjects()
	{
		sort(_objects.begin(),_objects.end(),imgSort);
	}
	void					renderObjects();

public:

	R2D_ImageRenderer(bool relativePos, ITexture* white, IVideoDevice* dev) : R2D_Renderer(R2D_Object::R2D_RECT,relativePos,white,dev)			{}
};

#endif