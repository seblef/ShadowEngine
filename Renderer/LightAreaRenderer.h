
#ifndef _LIGHTAREARENDERER_H_
#define _LIGHTAREARENDERER_H_

#include "LightRenderer.h"
#include "LightArea.h"

class LightAreaRenderer : public LightRenderer
{
protected:

	bool				isCameraInVolume(const Vector3& c, const Vector3& p, const Light *l) const
	{
		float d=(p-c).getSquareLength();
		float r=l->getRange();
		return d < r*r*5.0f;
	}

	void				fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const;

public:

	LightAreaRenderer(const LightRendererCreate_t& c);

};

#endif