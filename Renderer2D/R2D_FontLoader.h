
#ifndef _R2D_FONTLOADER_H_
#define _R2D_FONTLOADER_H_

#include "R2D_Font.h"

class R2D_FontLoader
{
public:

	R2D_FontLoader()			{}

	static R2D_Font*			loadFont(const string& fontFile, IVideoDevice* device);
};

#endif