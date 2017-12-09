
#ifndef _DX11MEDIA_H_
#define _DX11MEDIA_H_

#include "MediaCommon.h"
#include <Windows.h>

IMedia*			createDX11Media(HINSTANCE hInst, int width, int height, bool windowed, bool multithread, unsigned int input_flags);

#endif