
#ifndef _R2D_STDFRAMELOADER_H_
#define _R2D_STDFRAMELOADER_H_

#include "IR2D_FrameLoader.h"

class R2D_StdFrameLoader : IR2D_FrameLoader
{
protected:

	void									parseTexture(ScriptFile& sf, IVideoDevice* d) const;
	void									parseFont(ScriptFile& sf, IVideoDevice* d) const;
	void									parseLayer(ScriptFile& sf, R2D_Frame& f, IVideoDevice* d) const;
	void									parseLine(ScriptFile& sf, R2D_Layer& l) const;
	void									parseRect(ScriptFile& sf, R2D_Layer& l) const;
	void									parseImage(ScriptFile& sf, R2D_Layer& l) const;
	void									parseText(ScriptFile& sf, R2D_Layer& l) const;

public:

	R2D_StdFrameLoader()					{}

	R2D_Frame*								loadFrame(const string& frameFile, IVideoDevice* device);
};

#endif