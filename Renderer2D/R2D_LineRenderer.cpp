
#include "stdafx.h"
#include "R2D_LineRenderer.h"

void R2D_LineRenderer::renderObjects()
{
	_whiteTexture->set(0);

	ObjectVector::const_iterator o(_objects.begin());
	int o_count;
	while(o!=_objects.end())
	{
		o_count=0;
		while(o!=_objects.end() && o_count < R2D_MAX_OBJECTS)
		{
			makeLineVertices(**o,&_verts[o_count*2]);

			++o;
			++o_count;
		}

		_vBuffer->fill(_verts,o_count*2);
		_device->renderIndexed(o_count*2);
	}
}