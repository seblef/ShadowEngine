
#include "stdafx.h"
#include "R2D_StdFontLoader.h"
#include "FontFileFormat.h"


R2D_Font* R2D_StdFontLoader::loadFont(const string& fontFile, IVideoDevice* device)
{
	R2D_Font *f=0;
	ITexture* t;
	R2D_FontCharArea* a;

	IFile *fl=FileSystemFactory::getFileSystem()->openFile(fontFile,FA_READ);
	if(fl)
	{
		File::FontFileHeader h;
		fl->read(&h,sizeof(File::FontFileHeader));

		assert(h._header==FONT_HEADER);

#ifdef _UNICODE
		string file(h._texFile);
		t=device->createTexture(shString(file.begin(),file.end()));
#else
		t=device->createTexture(h._texFile);
#endif
		
		a=new R2D_FontCharArea[256];
		fl->read(a,sizeof(R2D_FontCharArea)*256);

		f=new R2D_Font(a,t);
		delete fl;
	}

	return f;
}