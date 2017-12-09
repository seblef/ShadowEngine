
#ifndef _G_BASETOOLCAMTRANSLATE_H_
#define _G_BASETOOLCAMTRANSLATE_H_

#include "G_Tool.h"
#include "G_Camera.h"
#include "G_EditorApp.h"

class G_BaseToolCamTranslate : public G_Tool
{
public:

	G_BaseToolCamTranslate()		{}

	void			mouseDown(int x, int y, unsigned short flags);
};

#endif