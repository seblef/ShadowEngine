
#ifndef _LIGHTOMNIRENDERER_H_
#define _LIGHTOMNIRENDERER_H_

#include "LightRenderer.h"

class LightOmniRenderer : public LightRenderer
{
protected:

	bool				isCameraInVolume(const Vector3& c, const Vector3& p, const Light *l) const
	{
		float r=l->getRange();
		return c.x > p.x - r && c.x < p.x + r &&
			   c.y > p.y - r && c.y < p.y + r &&
			   c.z > p.z - r && c.z < p.z + r;
	}

	void				fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const
	{
		float r=l->getRange();
		world.createScale(r,r,r);
		for(int i=0;i<3;++i)
			world(3,i)=l->getWorldMatrix()(3,i);
	}

public:

	LightOmniRenderer(const LightRendererCreate_t& c);

};

#endif