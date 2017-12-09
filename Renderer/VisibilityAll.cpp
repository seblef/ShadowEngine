
#include "stdafx.h"
#include "VisibilityAll.h"

void VisibilityAll::visibility(const ViewFrustum& vf, const SRect& bounds)
{
	RenderableSet::const_iterator r(_renderables.begin());
	for(;r!=_renderables.end();++r)
		(*r)->enqueue();
}