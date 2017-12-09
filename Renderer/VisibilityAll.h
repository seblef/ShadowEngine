
#ifndef _VISIBILITYALL_H_
#define _VISIBILITYALL_H_

#include "IVisibilitySystem.h"
#include <set>

class VisibilityAll : public IVisibilitySystem
{
protected:

	typedef set<Renderable*>		RenderableSet;
	RenderableSet					_renderables;

public:

	VisibilityAll()					{}
	~VisibilityAll()				{}

	void							create(int width, int height)				{}
	void							onResize(int w, int h)						{}

	void							addRenderable(Renderable* r)				{ _renderables.insert(r); }
	void							remRenderable(Renderable* r)				{ _renderables.erase(r); }

	void							onRenderableMoved(Renderable *r)			{}

	void							visibility(const ViewFrustum& vf, const SRect& bounds);
};

#endif