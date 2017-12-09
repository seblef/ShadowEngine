
#include "stdafx.h"
#include "G_EditorRenderer.h"
#include "../Game/GeometryLoader.h"


G_EditorRenderer::G_EditorRenderer(IVideoDevice *d) : _device(d)
{
	Geometry *g=GeometryLoader::loadGeometry("Editor/Box.geo");

	_shader=d->createShader("Editor/Objects");
	_vBuffer=d->createVertexBuffer(g->getVertexCount(),VX_3D,BU_IMMUTABLE,g->getVertices());
	_iBuffer=d->createIndexBuffer(g->getTriangleCount()*3,IDX_16BITS,BU_IMMUTABLE,g->getTriangles());

	_blendState=d->createBlendState(false,BLEND_ONE,BLEND_ZERO);
	_renderState=d->createRenderState(CULL_BACK);
	_depthState=d->createDepthStencilState(true,true,COMP_LESSEQUAL);
	_samplerState = d->createSamplerState(FILTER_TRILINEAR);

	const char * const g_EditorObjTextures[G_COUNT]={
		"Textures/Editor/Light.dds",
		"Textures/Editor/ParticleSystem.dds",
		"Textures/Editor/Sound.dds",
		"Textures/Editor/Trigger.dds"
	};

	for(int i=0;i<G_COUNT;++i)
		_textures[i]=d->createTexture(g_EditorObjTextures[i]);

	delete g;
}

G_EditorRenderer::~G_EditorRenderer()
{
	for(int i=0;i<G_COUNT;++i)
		delete _textures[i];

	_device->destroyDepthStencilState(_depthState);
	_device->destroyRenderState(_renderState);
	_device->destroyBlendState(_blendState);
	_device->destroySamplerState(_samplerState);

	delete _iBuffer;
	delete _vBuffer;
	delete _shader;
}

void G_EditorRenderer::render() const
{
	_device->resetRenderTargets();
	_shader->set();
	_vBuffer->set();
	_iBuffer->set();

	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);
	_device->setDepthStencilState(_depthState);
	_device->setSamplerState(0, _samplerState);

	_device->setInputLayer(VX_3D);
	_device->setPrimitiveType(PT_TRIANGLELIST);

	for(int i=0;i<G_COUNT;++i)
		renderObjects((GObjectType)i);
}

void G_EditorRenderer::renderObjects(GObjectType t) const
{
	_textures[t]->set(0);
	Matrix4 m(Matrix4::Identity);
//	m.createScale(0.5f,0.5f,0.5f);

	ObjectSet::const_iterator o(_objects[t].begin());
	for(;o!=_objects[t].end();++o)
	{
		m=(*o)->getPosition();
		Renderer::getSingletonRef().setObjectTransformation(m);
		_device->renderIndexed(_iBuffer->getIndexCount());
	}
}