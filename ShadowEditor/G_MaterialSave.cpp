
#include "stdafx.h"
#include "G_MaterialSave.h"
#include "G_EditorApp.h"

G_MaterialSave::G_MaterialSave(Material* m) : _mat(m),
	_buffer(m->getStdBuffer()), _flags(m->getFlag(MF_ALL))
{
	FillMode fm;

	for(int i=0;i<Material::TS_COUNT;++i)
	{
		if(!m->isDefaultTexture((Material::TextureSlot)i))
			_textures[i]=m->getTexture((Material::TextureSlot)i)->getName();
	}

	G_EditorApp::getSingletonRef().getVideo()->getBlendStateDesc(m->getBlendState(),_blendEnable,_srcBlend,_destBlend);
	G_EditorApp::getSingletonRef().getVideo()->getRenderStateDesc(m->getRenderState(),_cullMode,fm);

	for(int i=0;i<m->getAnimationCount();++i)
		_animations.getVector().push_back(MaterialAnimationFactory::copyAnimation(m->getAnimation(i)));
}

void G_MaterialSave::restore()
{
	_mat->getStdBuffer()=_buffer;

	_mat->unsetFlag(MF_ALL);
	_mat->setFlag(_flags);

	for(int i=0;i<Material::TS_COUNT;++i)
	{
		Material::TextureSlot ts=(Material::TextureSlot)i;
		if(_mat->isDefaultTexture(ts) && !_textures[i].empty() ||
			!_mat->isDefaultTexture(ts) && _textures[i]!=_mat->getTexture(ts)->getName())
			_mat->setTexture(ts,_textures[i]);
	}

	IMultiVideoDevice *d=G_EditorApp::getSingletonRef().getVideo();
	d->destroyBlendState(_mat->getBlendState());
	d->destroyRenderState(_mat->getRenderState());

	_mat->setBlendState(d->createBlendState(_blendEnable,_srcBlend,_destBlend));
	_mat->setRenderState(d->createRenderState(_cullMode,FILL_SOLID));

	while(_mat->getAnimationCount() > 0)
		_mat->deleteAnimation(0);

	for(int i=0;i<_animations.getVector().size();++i)
		_mat->addAnimation(MaterialAnimationFactory::copyAnimation(_animations.getVector()[i]));
}