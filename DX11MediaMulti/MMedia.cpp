
#include "stdafx.h"
#include "MMedia.h"
#include "MVideoDevice.h"
#include "../DX11Media/SoundDevice.h"


IMedia*			createDX11MediaMulti(HINSTANCE hInst)
{
	return new MMedia(hInst);
}


MMedia::MMedia(HINSTANCE hInst)
{
	_video=new MVideoDevice(hInst);
	_audio = new SoundDevice;
}
