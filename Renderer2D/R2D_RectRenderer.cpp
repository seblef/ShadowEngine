
#include "stdafx.h"
#include "R2D_RectRenderer.h"

void R2D_RectRenderer::renderObjects()
{
	_whiteTexture->set(0);

	ObjectVector::const_iterator o(_objects.begin());
	int o_count;
	while(o!=_objects.end())
	{
		o_count=0;
		while(o!=_objects.end() && o_count < R2D_MAX_OBJECTS)
		{
			makeRectVertices(**o,&_verts[o_count*4]);

			++o;
			++o_count;
		}

		_vBuffer->fill(_verts,o_count*4);
		_device->renderIndexed(o_count*6);
	}
}