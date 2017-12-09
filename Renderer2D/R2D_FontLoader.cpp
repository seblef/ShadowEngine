
#include "stdafx.h"
#include "R2D_FontLoader.h"
#include "R2D_StdFontLoader.h"

R2D_Font* R2D_FontLoader::loadFont(const string& fontFile, IVideoDevice* device)
{
	R2D_StdFontLoader loader;
	return loader.loadFont(fontFile,device);
}