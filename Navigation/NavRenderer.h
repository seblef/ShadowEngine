
#ifndef _NAVRENDERER_H_
#define _NAVRENDERER_H_

#include <MediaCommon.h>
#include "NavPath.h"

class NavRenderer : public TSingleton<NavRenderer>
{
protected:

	IVideoDevice*				_device;
	IShader*					_shader;
	IVertexBuffer*				_vBuffer;
	BlendState					_blendState;
	RenderState					_renderState;
	DepthStencilState			_depthState;

	NavPath*					_path;
	TDynamicTab<VxBase>			_vertices;

	bool						_mapEnable;

public:

	NavRenderer(IVideoDevice *d);
	~NavRenderer();

	void						renderNavMap();
	void						setNavPath(NavPath *p)				{ _path=p; }

	void						enableFullMap(bool enable)			{ _mapEnable = enable; }
};

#endif