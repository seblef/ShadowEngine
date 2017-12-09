
#ifndef _MEDIA_H_
#define _MEDIA_H_

#include <MediaCommon.h>
#include <Windows.h>

class MMedia : public IMedia
{
public:

	MMedia(HINSTANCE hInst);
	~MMedia()										{}

	void				update()					{}
};

#endif