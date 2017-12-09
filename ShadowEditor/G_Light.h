
#ifndef _G_LIGHT_H_
#define _G_LIGHT_H_

#include "G_Object.h"
#include "G_EditorRenderer.h"
#include <StdRenderer.h>

const char* const g_LightTypes[Light::LT_COUNT]=
{
	_T("omni"),
	_T("spot"),
	_T("area")
};

class G_Light : public G_Object
{
protected:

	Light*				_rLight;
	Vector3				_originalDir;

	void				rebuildMatrix();

	void				drawOmni(G_EditorObjDrawer* d) const;
	void				drawSpot(G_EditorObjDrawer* d) const;
	void				drawArea(G_EditorObjDrawer* d) const;

public:

	G_Light(Light::LightType type, const LightCreate_t& c);
	G_Light(Light::LightType type);
	G_Light(const G_Light& l);
	~G_Light()
	{
		delete _rLight;
	}

	const Light&		getLight() const				{ return *_rLight; }
	Light&				getLight()						{ return *_rLight; }

	const LightOmni&	getOmni() const					{ return *((LightOmni*)_rLight); }
	const LightSpot&	getSpot() const					{ return *((LightSpot*)_rLight); }
	const LightArea&	getArea() const					{ return *((LightArea*)_rLight); }

	void				onAddToScene()
	{
		G_Object::onAddToScene();
		G_EditorRenderer::getSingletonRef().addObject(this);
		Renderer::getSingletonRef().addRenderable(_rLight);
	}
	void				onRemFromScene()
	{
		G_Object::onRemFromScene();
		G_EditorRenderer::getSingletonRef().remObject(this);
		Renderer::getSingletonRef().remRenderable(_rLight);
	}

	void				drawSelection(G_EditorObjDrawer* d);

	G_Object*			copy()							{ return new G_Light(*this); }
};

#endif