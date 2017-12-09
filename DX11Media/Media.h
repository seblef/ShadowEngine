
#ifndef _MEDIA_H_
#define _MEDIA_H_

#include <MediaCommon.h>
#include "WinWindow.h"
#include "Input.h"

class Media : public IMedia
{
protected:

	WinWindow*			_win;
	Input*				_input;

public:

	Media(HINSTANCE hInst, int width, int height, bool windowed, unsigned int input_flags);
	~Media();

	void				update();
};

#endif