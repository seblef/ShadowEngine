
#include "stdafx.h"
#include "ParticleTextureSet.h"
#include "ParticleTextureStore.h"
#include <algorithm>

ParticleTextureSet::ParticleTextureSet(ScriptFile& sf)
{
	string token(sf.getToken());

	while(token!="end_texture_set")
	{
		addTexture(token);
		token=sf.getToken();
	}
}

ParticleTextureSet::ParticleTextureSet(const ParticleTextureSet& s)
{
	for (int i = 0; i < s.getTextureCount(); ++i)
		addTexture(s.getTexture(i)->getName());
}

ParticleTextureSet::ParticleTextureSet(const string& dir)
{
	IFileSystem *sys = FileSystemFactory::getFileSystem();
	const char * textureExts[] = {
		"dds",
		"bmp",
		"jpg",
		"png",
		"tga"
	};

	sys->resetWorkingDirectory();
	sys->changeWorkingDirectory(dir);

	IFileList *l = sys->createFileList((const char**)textureExts, 5, true);
	l->sortFiles();

	for (int i = 0; i < l->getFileCount(); ++i)
		addTexture(l->getFile(i));

	sys->resetWorkingDirectory();
	delete l;
}

ParticleTextureSet::~ParticleTextureSet()
{
	TexVector::iterator t(_textures.begin());
	for(;t!=_textures.end();++t)
		ParticleTextureStore::getSingletonRef().releaseTexture((*t)->getName());
}

void ParticleTextureSet::addTexture(const string& texFile)
{
	ITexture *t=ParticleTextureStore::getSingletonRef().getTexture(texFile);
	if(t)
		_textures.push_back(t);
}
