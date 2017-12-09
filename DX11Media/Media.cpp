
#include "stdafx.h"
#include "Media.h"
#include "VideoDevice.h"
#include "SoundDevice.h"


IMedia*			createDX11Media(HINSTANCE hInst, int width, int height, bool windowed, bool multithread, unsigned int input_flags)
{
	return new Media(hInst,width,height,windowed,input_flags);
}


Media::Media(HINSTANCE hInst, int width, int height, bool windowed, unsigned int input_flags)
{
	_win=new WinWindow(hInst,width,height,_T("Shadow"));
	_input=new Input(*_win,input_flags);
	_video=new VideoDevice(*_win,windowed);
	_audio = new SoundDevice;
}

Media::~Media()
{
	delete _input;
	delete _win;
}

void Media::update()
{
	MSG msg;

	if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		GetMessage(&msg,NULL,0,0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	_input->update();

	if (_audio)
		_audio->update();
}