
#ifndef _ACTORSKINNEDRENDERER_H_
#define _ACTORSKINNEDRENDERER_H_

#include "TRenderer.h"
#include "ActorInstance.h"

class ActorSkinnedRenderer : public TRenderer<ActorInstance>, public TSingleton<ActorSkinnedRenderer>
{
protected:

	IConstantBuffer*		_boneCBuffer;

public:

	ActorSkinnedRenderer(IVideoDevice* device);
	~ActorSkinnedRenderer();

	void					render(bool addPass);
	void					renderShadows(const ViewFrustum& vf);
};

#endif