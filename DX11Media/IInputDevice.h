
#ifndef _IINPUTDEVICE_H_
#define _IINPUTDEVICE_H_

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


class IInputDevice
{
protected:

	IDirectInputDevice8*		_device;

	bool								buttonDown(BYTE b) const			{ return (b & 0x80)!=0; }
	bool								buttonUp(BYTE b) const				{ return !buttonDown(b); }
	
public:

	IInputDevice() : _device(0)			{}
	virtual ~IInputDevice()				{}

	virtual void				update()=0;
};

#endif