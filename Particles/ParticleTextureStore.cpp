
#include "stdafx.h"
#include "ParticleTextureStore.h"

ParticleTextureStore::~ParticleTextureStore()
{
	TextureMap::iterator t(_textures.begin());
	for(;t!=_textures.end();++t)
		t->second.clear();
}

ITexture* ParticleTextureStore::getTexture(const string& texFile)
{
	TextureMap::iterator t(_textures.find(texFile));
	if(t==_textures.end())
	{
		ITexture *tex=_device->createTexture(texFile);
		if(tex)
		{
			_textures[texFile]=PTSItem(tex);
			return tex;
		}
		else
			return 0;
	}
	else
	{
		t->second.addRef();
		return t->second;
	}
}

void ParticleTextureStore::releaseTexture(const string& texFile)
{
	TextureMap::iterator t(_textures.find(texFile));
	if(t!=_textures.end())
	{
		if(t->second.remRef() <= 0)
			_textures.erase(t);
	}
}