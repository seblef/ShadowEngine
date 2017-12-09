
#ifndef _ACTORRENDERER_H_
#define _ACTORRENDERER_H_

#include "TRenderer.h"
#include "ActorInstance.h"

class ActorRenderer : public TRenderer<ActorInstance>, public TSingleton<ActorRenderer>
{
public:

	ActorRenderer(IVideoDevice* device) : TRenderer<ActorInstance>(device,"Base/GBuffer","Base/Add","Base/Shadows")			{}
	~ActorRenderer()													{}

	void					render(bool addPass);
	void					renderShadows(const ViewFrustum& vf);
};

#endif