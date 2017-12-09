
#ifndef _HUDACTORVIEW_H_
#define _HUDACTORVIEW_H_

#include <MediaCommon.h>

class HUDActor;

class HUDActorView
{
protected:

	IVideoDevice*		_device;
	IShader*			_shader;
	IVertexBuffer*		_vBuffer;
	BlendState			_blendState;
	RenderState			_renderState;
	DepthStencilState	_dsState;
	TDynamicTab<VxBase>	_vertices;

public:

	HUDActorView(IVideoDevice* device);
	~HUDActorView();

	void				renderHUD(Camera& c, const set<HUDActor*>& actors);
};

#endif