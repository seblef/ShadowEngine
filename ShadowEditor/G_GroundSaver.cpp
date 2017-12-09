
#include "stdafx.h"
#include "G_GroundSaver.h"
#include "../Game/GroundFileFormat.h"

void G_GroundSaver::saveGround(G_Ground& g, const string& fileName) const
{
	IFile *f=FileSystemFactory::getFileSystem()->openFile(fileName,FA_WRITE);

	File::GroundFileHeader h;
	File::GroundFileMaterial m;

	h._header=GROUND_HEADER;
	h._width=g.getWidth();
	h._height=g.getHeight();
	h._matCount=g.getMaterialCount();
	h._gameMatCount = g.getGameMaterialCount() - 1;

	f->write(&h,sizeof(File::GroundFileHeader));

	const map<string,Material*>& db(MaterialSystem::getSingletonRef().getMaterials().getData());
	map<string,Material*>::const_iterator mat;

	for(int i=0;i<h._matCount;++i)
	{
		for(mat=db.begin();mat!=db.end();++mat)
			if(mat->second==&g.getMaterial(i))
				strcpy_s(m._name,mat->first.c_str());

		f->write(&m,sizeof(File::GroundFileMaterial));
	}

	for (int i = 0; i < h._gameMatCount; ++i)
	{
		strcpy_s(m._name, g.getGameMaterial(i+1)->getName().c_str());
		f->write(&m, sizeof(File::GroundFileMaterial));
	}

	unsigned int *gr=new unsigned int[g.getWidth()*g.getHeight()];
	for(int y=0;y<g.getHeight();++y)
		for(int x=0;x<g.getWidth();++x)
			gr[x+y*h._width]=(unsigned int)(g.getTriangle(x,y,0).getMatID()+1) | (g.getGameMaterial(x,y) << 8) | g.getFlag(x,y,0xffffffff);

	f->write(gr,sizeof(unsigned int) * h._width * h._height);

	delete[] gr;
	delete f;
}