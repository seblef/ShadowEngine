
#ifndef _IMULTIVIDEODEVICE_H_
#define _IMULTIVIDEODEVICE_H_

#include "IVideoDevice.h"
#include "IView.h"

class IMultiVideoDevice : public IVideoDevice
{
public:

	IMultiVideoDevice(int width, int height, bool win) : IVideoDevice(width,height,win)			{}
	virtual ~IMultiVideoDevice()																{}

	virtual IView*				createView(int w, int h, void* param)=0;
	virtual void				setCurrentView(IView* v)=0;
	virtual void				resizeView(IView* v, int w, int h)=0;
	virtual void				destroyView(IView *v)=0;
};

#endif