
#ifndef _R2D_RECTRENDERER_H_
#define _R2D_RECTRENDERER_H_

#include "R2D_Renderer.h"

class R2D_RectRenderer : public R2D_Renderer
{
protected:

	void					renderObjects();

public:

	R2D_RectRenderer(bool relativePos, ITexture* white, IVideoDevice* dev) : R2D_Renderer(R2D_Object::R2D_RECT,relativePos,white,dev)			{}
};

#endif