
#ifndef _G_FLAGRENDERER_H_
#define _G_FLAGRENDERER_H_

#include <MediaCommon.h>

class G_FlagRenderer
{
protected:

	IVideoDevice*		_device;
	IShader*			_shader;
	IVertexBuffer*		_vBuffer;
	BlendState			_blendState;
	DepthStencilState	_depthState;
	RenderState			_renderState;

	TDynamicTab<VxBase>	_vertices;

	unsigned int		_flags;

public:

	G_FlagRenderer(IVideoDevice *d);
	~G_FlagRenderer();

	void				setFlag(unsigned int f)				{ _flags=f; }

	void				render();
};

#endif