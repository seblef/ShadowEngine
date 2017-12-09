
#ifndef _MOUSEDEVICE_H_
#define _MOUSEDEVICE_H_

#include "IInputDevice.h"
#include "WinWindow.h"

class MouseDevice : public IInputDevice
{
private:

	DIMOUSESTATE						_lastState;

public:

	MouseDevice(IDirectInput8 *di, const WinWindow& win);
	~MouseDevice();

	void								update();
};

#endif