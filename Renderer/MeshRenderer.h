
#ifndef _MESHRENDERER_H_
#define _MESHRENDERER_H_

#include "TRenderer.h"
#include "MeshInstance.h"

class MeshRenderer : public TRenderer<MeshInstance>, public TSingleton<MeshRenderer>
{
public:

	MeshRenderer(IVideoDevice* device) : TRenderer<MeshInstance>(device,_T("Base/GBuffer"),_T("Base/Add"),_T("Base/Shadows"))		{}
	~MeshRenderer()													{}

	void					render(bool addPass);
	void					renderShadows(const ViewFrustum& vf);
};

#endif