
#include "stdafx.h"
#include "G_GroundLoader.h"
#include "G_EditorApp.h"
#include "../Game/GroundFileFormat.h"


void G_GroundLoader::loadGround(const string& groundFile, G_Ground& g, G_Map& mp) const
{
	IFile* f=FileSystemFactory::getFileSystem()->openFile(groundFile,FA_READ);
	File::GroundFileHeader h;
	File::GroundFileMaterial m;
	f->read(&h,sizeof(File::GroundFileHeader));

	assert(h._header==GROUND_HEADER && h._width==g.getWidth() && h._height==g.getHeight());

	Material **mm=new Material*[h._matCount];
	unsigned int *ids=new unsigned int[h._width*h._height];

	G_GameMaterial** gmm = 0;
	if (h._gameMatCount > 0)
	{
		gmm = new G_GameMaterial*[h._gameMatCount];
		memset(gmm, 0, sizeof(G_GameMaterial*)*h._gameMatCount);
	}

	for(int i=0;i<h._matCount;++i)
	{
		f->read(&m,sizeof(File::GroundFileMaterial));
		mm[i]=MaterialSystem::getSingletonRef().getMaterial(m._name);
	}

	for (int i = 0; i < h._gameMatCount; ++i)
	{
		f->read(&m, sizeof(File::GroundFileMaterial));
		gmm[i] = mp.getGameMaterials().getData(m._name);
	}

	f->read(ids,sizeof(unsigned int) * h._width * h._height);
	g.onLoad(h._matCount, mm, ids, h._gameMatCount, gmm);

	delete[] ids;
	if (gmm)
		delete[] gmm;
	delete f;
}