
#include "stdafx.h"
#include "R2D_ImageRenderer.h"
#include <algorithm>

void R2D_ImageRenderer::renderObjects()
{
	ObjectVector::const_iterator o(_objects.begin());
	int o_count;
	ITexture *t;
	const R2D_Image* img;
	while(o!=_objects.end())
	{
		t=((const R2D_Image*)*o)->getTexture();
		t->set(0);

		while(o!=_objects.end() && ((const R2D_Image*)*o)->getTexture()==t)
		{
			o_count=0;
			while(o!=_objects.end() && ((const R2D_Image*)*o)->getTexture()==t && o_count < R2D_MAX_OBJECTS)
			{
				img=(const R2D_Image*)*o;
				makeRectVertices(*img,img->getUVStart(), img->getUVStart() + img->getUVSize(), &_verts[o_count*4]);

				++o;
				++o_count;
			}

			_vBuffer->fill(_verts,o_count*4);
			_device->renderIndexed(o_count*6);
		}
	}
}