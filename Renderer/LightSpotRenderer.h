
#ifndef _LIGHTSPOTRENDERER_H_
#define _LIGHTSPOTRENDERER_H_

#include "LightRenderer.h"
#include "LightSpot.h"

class LightSpotRenderer : public LightRenderer
{
protected:

	bool				isCameraInVolume(const Vector3& c, const Vector3& p, const Light *l) const
	{
		float d=(p-c).getSquareLength();
		float r=l->getRange();
		return d < r*r*2.0f;
	}

	void				fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const
	{
		const LightSpot *s=(const LightSpot*)l;
		float sfar=s->getFarAngle() * M_PI / 360.0f;
		float snear=s->getNearAngle() * M_PI / 360.0f;

		float cf=cosf(sfar);
		float sf=sinf(sfar);

		b._nearAngle=cosf(snear);
		b._farAngle=cf;
		b._dir=s->getDirection();

		float xyScale=l->getRange() * sf / cf;
		world.createScale(xyScale,xyScale,l->getRange());
		world*=l->getWorldMatrix();
	}

public:

	LightSpotRenderer(const LightRendererCreate_t& c);

};

#endif