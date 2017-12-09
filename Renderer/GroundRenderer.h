
#ifndef _GROUNDRENDERER_H_
#define _GROUNDRENDERER_H_

#include "GroundData.h"
#include <MediaCommon.h>
#include <vector>

using namespace std;

class GroundRenderer : public TSingleton<GroundRenderer>
{
protected:

	IVideoDevice*					_device;
	IVertexBuffer*					_VBuffer;
	Vx3D*							_buffer;

	BlendState						_bsWakable;
	DepthStencilState				_dsWakable;
	RenderState						_rsWakable;
	IShader*						_flagShader;

	typedef vector<const GroundTri*>		TriVector;
	TriVector						_triangles;

	static bool						sortTriangles(const GroundTri* t1, const GroundTri* t2)			{ return (*t1) < (*t2); }

public:

	GroundRenderer(IVideoDevice *device);
	~GroundRenderer();

	void				visibility(const GroundData& data, const SRect& bounds);
	void				renderGround(const GroundData& data, bool addPass);
	void				renderFlag(unsigned int f, const unsigned int* flagMap, int w);
};

#endif