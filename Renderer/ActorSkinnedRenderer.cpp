
#include "stdafx.h"
#include "ActorSkinnedRenderer.h"
#include "Renderer.h"

extern ofstream				g_MediaOut;


ActorSkinnedRenderer::ActorSkinnedRenderer(IVideoDevice* device) : TRenderer<ActorInstance>(device,_T("Base/GBufferSkin"),_T("Base/AddSkin"),_T("Base/ShadowsSkin"))
{
	_boneCBuffer=device->createConstantBuffer(MAX_ACTOR_BONES*4,3);
}

ActorSkinnedRenderer::~ActorSkinnedRenderer()
{
	delete _boneCBuffer;
}

void ActorSkinnedRenderer::render(bool addPass)
{
	_device->setInputLayer(VX_3DSKIN);
	_boneCBuffer->set(ST_VERTEX);

	if(addPass)		_addShader->set();
	else			_gbufferShader->set();

	Material *m;
	Matrix4 buffer[MAX_ACTOR_BONES];
	const Actor *mesh;
	int ind_count;
	Renderer& r=Renderer::getSingletonRef();
	r.setObjectTransformation(Matrix4::Identity);

	InstanceVector::const_iterator i(_instances.begin());
	while(i!=_instances.end())
	{
		m=(*i)->getModel()->getMaterial();
		if(addPass)
			m->setAdd((*i)->getTime());
		else
			m->setBase((*i)->getTime());

		while(i!=_instances.end() && (*i)->getModel()->getMaterial()==m)
		{
			mesh=(*i)->getModel();
			mesh->getVertexBuffer()->set();
			mesh->getIndexBuffer()->set();
			ind_count=mesh->getIndexBuffer()->getIndexCount();
//			r.setObjectTransformation(mesh->getInvSkinMatrix());

			while(i!=_instances.end() && (*i)->getModel()->getMaterial()==m && (*i)->getModel()==mesh)
			{
				m=(*i)->getModel()->getMaterial();
				if(m->isAnimated())
					m->setTime((*i)->getTime());

				memcpy(buffer,(*i)->getBoneMatrices()->getItems(),(*i)->getBoneMatrices()->getCount()*sizeof(Matrix4));
				_boneCBuffer->fill((const void*)buffer);
				_device->renderIndexed(ind_count);

				++i;
			}
		}
	}
}

void ActorSkinnedRenderer::renderShadows(const ViewFrustum& vf)
{
	_device->setInputLayer(VX_3DSKIN);
	_boneCBuffer->set(ST_VERTEX);
	_shadowShader->set();

	Matrix4 buffer[MAX_ACTOR_BONES];
	const Actor *mesh;
	int ind_count;
	Renderer& r=Renderer::getSingletonRef();
	r.setObjectTransformation(Matrix4::Identity);

	InstanceVector::const_iterator i(_instances.begin());
	while(i!=_instances.end())
	{
		mesh=(*i)->getModel();

		if(mesh->getMaterial()->getFlag(MF_CASTNOSHADOWS))
		{
			while(i!=_instances.end() && (*i)->getModel()==mesh)
				++i;
		}
		else
		{
			mesh->getVertexBuffer()->set();
			mesh->getIndexBuffer()->set();
			ind_count=mesh->getIndexBuffer()->getIndexCount();
//			r.setObjectTransformation(mesh->getInvSkinMatrix());

			while(i!=_instances.end() && (*i)->getModel()==mesh)
			{
				memcpy(buffer,(*i)->getBoneMatrices()->getItems(),(*i)->getBoneMatrices()->getCount()*sizeof(Matrix4));
				_boneCBuffer->fill((const void*)buffer);
				_device->renderIndexed(ind_count);

				++i;
			}
		}
	}
}