
#include "stdafx.h"
#include "R2D_FrameLoader.h"
#include "R2D_StdFrameLoader.h"


R2D_Frame* R2D_FrameLoader::loadFrame(const string& frameFile, IVideoDevice* d)
{
	R2D_StdFrameLoader loader;
	return loader.loadFrame(frameFile,d);
}