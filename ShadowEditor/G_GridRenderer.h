
#ifndef _G_GRIDRENDERER_H_
#define _G_GRIDRENDERER_H_

#include <MediaCommon.h>

class G_GridRenderer
{
protected:

	IVideoDevice*		_device;
	IShader*			_shader;
	IVertexBuffer*		_vBuffer;
	BlendState			_blendState;
	DepthStencilState	_depthState;

public:

	G_GridRenderer(IVideoDevice* device);
	~G_GridRenderer();

	void				render();
};

#endif