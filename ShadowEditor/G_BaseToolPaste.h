
#ifndef _G_BASETOOLPASTE_H_
#define _G_BASETOOLPASTE_H_

#include "G_BaseToolTranslate.h"
#include "G_Object.h"

class G_BaseToolPaste : public G_BaseToolTranslate
{
public:

	G_BaseToolPaste()			{}

	void					mouseDown(int x, int y, unsigned short flags);
	void					mouseUp(int x, int y);

	void					cancel();
};

#endif