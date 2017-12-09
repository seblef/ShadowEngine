
#include "stdafx.h"
#include "MeshRenderer.h"
#include "Renderer.h"

extern ofstream		g_MediaOut;

void MeshRenderer::render(bool addPass)
{
	Material *m;
	const Mesh *mesh;
	int ind_count;
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
			ind_count=mesh->getIndexBuffer()->getIndexCount();

			while(i!=_instances.end() && (*i)->getModel()->getMaterial()==m && (*i)->getModel()==mesh)
			{
				m=(*i)->getModel()->getMaterial();
				if(m->isAnimated())
					m->setTime((*i)->getTime());
	
				r.setObjectTransformation((*i)->getWorldMatrix());
				_device->renderIndexed(ind_count);

				++i;
			}
		}
	}
}

void MeshRenderer::renderShadows(const ViewFrustum& vf)
{
	_device->setInputLayer(VX_3D);
	_shadowShader->set();

	const Mesh* mesh;
	int ind_count;
	Renderer& r(Renderer::getSingletonRef());

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

			while(i!=_instances.end() && (*i)->getModel()==mesh)
			{
				if(vf.isBoxInside((*i)->getWorldBBox()))
				{
					r.setObjectTransformation((*i)->getWorldMatrix());
					_device->renderIndexed(ind_count);
				}

				++i;
			}
		}
	}
}