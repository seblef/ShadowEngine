
#ifndef _G_EDITORRENDERER_H_
#define _G_EDITORRENDERER_H_

#include <DX11MediaMulti.h>
#include "G_Object.h"

class G_EditorRenderer : public TSingleton<G_EditorRenderer>
{
protected:

	enum GObjectType
	{
		G_LIGHT=0,
		G_PARTICLESYSTEM,
		G_SOUND,
		G_ENTITY,

		G_COUNT
	};

	IVideoDevice*			_device;
	IShader*				_shader;
	IVertexBuffer*			_vBuffer;
	IIndexBuffer*			_iBuffer;
	BlendState				_blendState;
	RenderState				_renderState;
	DepthStencilState		_depthState;
	SamplerState			_samplerState;

	ITexture*				_textures[G_COUNT];

	typedef set<G_Object*>	ObjectSet;
	ObjectSet				_objects[G_COUNT];

	GObjectType				getType(const G_Object* o) const
	{
		GObjectType t=G_COUNT;
		switch(o->getType())
		{
		case G_Object::LIGHT:				t=G_LIGHT;				break;
		case G_Object::PARTICLESYSTEM:		t = G_PARTICLESYSTEM;	break;
		case G_Object::SOUND:				t = G_SOUND;			break;
		case G_Object::ENTITY:				t = G_ENTITY;			break;
		};

		return t;
	}

	void					renderObjects(GObjectType t) const;

public:

	G_EditorRenderer(IVideoDevice* d);
	~G_EditorRenderer();

	void					addObject(G_Object* o)			{ _objects[getType(o)].insert(o); }
	void					remObject(G_Object* o)			{ _objects[getType(o)].erase(o); }

	void					render() const;
};

#endif