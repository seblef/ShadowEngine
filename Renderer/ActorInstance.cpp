
#include "stdafx.h"
#include "ActorInstance.h"
#include "ActorRenderer.h"
#include "MeshRenderer.h"
#include "ActorSkinnedRenderer.h"

extern ofstream g_MediaOut;

ActorInstance::ActorInstance(Actor* model, const Matrix4& world, bool alwaysVisible) : Renderable(model->getBBox(),world,alwaysVisible),
	_pose(model->getInitialPose()), _worldMatrices(model->getInitialPose()), _model(model), _animation(model->getInitialPose().getCount()),
	_nullNodesMeshes(0)
{
	if(model->isSkinned())
		_boneMatrices=new MatrixTab(model->getBoneMatrices().getCount());
	else
		_boneMatrices=0;

	if(model->getNullNodeCount())
	{
		_nullNodesMeshes=new MeshInstance*[model->getNullNodeCount()];
		for(int i=0;i<model->getNullNodeCount();++i)
			_nullNodesMeshes[i]=0;
	}

	update(0);
}

void ActorInstance::enqueue()
{
	if(_boneMatrices)
		ActorSkinnedRenderer::getSingletonRef().addInstance(this);
	else
		ActorRenderer::getSingletonRef().addInstance(this);

	for(int i=0;i<_model->getNullNodeCount();++i)
	{
		if(_nullNodesMeshes[i])
			_nullNodesMeshes[i]->enqueue();
	}
}

void ActorInstance::update(float time)
{
	IUpdatable::update(time);

	_animation.update(time,_pose);
	_worldMatrices.update(_worldMatrix,_pose);

	if(_boneMatrices)
	{
		for(int b=0;b<_boneMatrices->getCount();++b)
			_boneMatrices->getItem(b)=_model->getBoneMatrices().getItem(b) * _worldMatrices.getItem(b);
	}

	for(int i=0;i<_model->getNullNodeCount();++i)
	{
		if(_nullNodesMeshes[i])
			_nullNodesMeshes[i]->setWorldMatrix(_worldMatrices.getItem(_model->getNullNode(i).getID()));
	}
}
