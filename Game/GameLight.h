
#ifndef _GAMELIGHT_H_
#define _GAMELIGHT_H_

#include "GameObject.h"
#include <StdRenderer.h>

class GameLight : public GameObject
{
protected:

	Light*					_rLight;

public:

	GameLight(const LightCreate_t& c, Light::LightType type) : GameObject(LIGHT,c._world,BBox3(-c._range,-c._range,-c._range,c._range,c._range,c._range),OF_NOCOLLISION)
	{
		switch (type)
		{
		case Light::LT_OMNI:
			_rLight=new LightOmni(c);
			break;
		case Light::LT_SPOT:
			_rLight=new LightSpot(c);
			break;
		case Light::LT_AREA:
			_rLight=new LightArea(c);
			break;

		default:
			_rLight=new LightOmni(c);
		}
	}
	~GameLight()
	{
		delete _rLight;
	}

	void					onAddToScene()					{ Renderer::getSingletonRef().addRenderable(_rLight); }
	void					onRemFromScene()				{ Renderer::getSingletonRef().remRenderable(_rLight); }
	void					onUpdateMatrix()				{ _rLight->setWorldMatrix(_world); }
};

#endif