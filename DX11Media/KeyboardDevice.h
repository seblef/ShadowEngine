
#ifndef _KEYBOARDDEVICE_H_
#define _KEYBOARDDEVICE_H_

#include "IInputDevice.h"
#include "WinWindow.h"

class KeyboardDevice : public IInputDevice
{
protected:

	unsigned char		_lastKeys[256];

public:

	KeyboardDevice(IDirectInput8* di, const WinWindow& win);
	~KeyboardDevice();

	void				update();
};

#endif