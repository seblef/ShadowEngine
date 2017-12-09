
#include "stdafx.h"
#include "ActorRenderer.h"
#include "Renderer.h"

void ActorRenderer::render(bool addPass)
{
	Material *m;
	const Actor *mesh;
	ActorInstance* a;
	Renderer& r=Renderer::getSingletonRef();

	_device->setInputLayer(VX_3D);
	if(addPass)				_addShader->set();
	else					_gbufferShader->set();

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

			while(i!=_instances.end() && (*i)->getModel()->getMaterial()==m && (*i)->getModel()==mesh)
			{
				m=(*i)->getModel()->getMaterial();
				if(m->isAnimated())
					m->setTime((*i)->getTime());

				a=*i;
				for(int s=0;s<a->getWorldMatrices().getCount();++s)
				{
					r.setObjectTransformation(a->getWorldMatrices().getItem(s));
					_device->renderIndexed(mesh->getSub(s).getTriCount()*3,
						mesh->getSub(s).getTriOffset()*3);
				}

				++i;
			}
		}
	}
}

void ActorRenderer::renderShadows(const ViewFrustum& vf)
{
	const Actor *mesh;
	ActorInstance* a;
	Renderer& r=Renderer::getSingletonRef();

	_device->setInputLayer(VX_3D);
	_shadowShader->set();

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

			while(i!=_instances.end() && (*i)->getModel()==mesh)
			{
				if(vf.isBoxInside((*i)->getWorldBBox()))
				{
					a=*i;
					for(int s=0;s<a->getWorldMatrices().getCount();++s)
					{
						r.setObjectTransformation(a->getWorldMatrices().getItem(s));
						_device->renderIndexed(mesh->getSub(s).getTriCount()*3,
							mesh->getSub(s).getTriOffset()*3);
					}
				}

				++i;
			}
		}
	}
}