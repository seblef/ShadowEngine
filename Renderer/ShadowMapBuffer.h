
#ifndef _SHADOWMAPBUFFER_H_
#define _SHADOWMAPBUFFER_H_

#include <MediaCommon.h>

class ShadowMapBuffer
{
protected:

	Matrix4					_viewProj;
	Vector2					_invShadowMapSize;
	float					_shadowFilter;
	float					_invShadowFilter;

	IConstantBuffer*		_cBuffer;

public:

	ShadowMapBuffer(IVideoDevice* device)
	{
		_cBuffer=device->createConstantBuffer(5,3);
	}

	~ShadowMapBuffer()
	{
		delete _cBuffer;
	}

	void					set()					{ _cBuffer->set(ST_PIXEL); }
	void					setupLight(IShadowMap *m, int filter, const Matrix4& viewProj)
	{
		int f=(int)filter;
		_viewProj=viewProj;
		_invShadowMapSize=Vector2(1.0f / m->getWidth(),1.0f / m->getHeight());
		_shadowFilter=*((float*)&f);
		_invShadowFilter=1.0f / filter;
		_cBuffer->fill(this);
	}
};

#endif