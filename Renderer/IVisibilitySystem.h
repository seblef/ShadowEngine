
#ifndef _IVISIBILITYSYSTEM_H_
#define _IVISIBILITYSYSTEM_H_

#include "Renderable.h"

class IVisibilitySystem
{
public:

	IVisibilitySystem()				{}
	virtual ~IVisibilitySystem()	{}

	virtual void				create(int width, int height)=0;
	virtual void				onResize(int w, int h)=0;

	virtual void				addRenderable(Renderable *r)=0;
	virtual void				remRenderable(Renderable *r)=0;

	virtual void				onRenderableMoved(Renderable* r)=0;

	virtual void				visibility(const ViewFrustum& vf, const SRect& bounds)=0;
};

#endif