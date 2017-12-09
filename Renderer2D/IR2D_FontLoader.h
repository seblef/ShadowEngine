
#ifndef _IR2D_FONTLOADER_H_
#define _IR2D_FONTLOADER_H_

#include "R2D_Font.h"

class IR2D_FontLoader
{
public:

	IR2D_FontLoader()				{}
	virtual ~IR2D_FontLoader()		{}

	virtual R2D_Font*				loadFont(const string& fontFile, IVideoDevice* device)=0;
};

#endif