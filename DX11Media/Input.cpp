
#include "stdafx.h"
#include "Input.h"
#include "MouseDevice.h"
#include "KeyboardDevice.h"

Input::Input(const WinWindow& win, unsigned int flags)
{
	DirectInput8Create(win.getHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8A,
							(LPVOID*)&_di,NULL);

	if(flags & SINPUT_MOUSE)
		_devices.push_back(new MouseDevice(_di,win));

	if(flags & SINPUT_KEYBOARD)
		_devices.push_back(new KeyboardDevice(_di,win));
}

Input::~Input()
{
	DeviceVector::const_iterator d(_devices.begin());
	for(;d!=_devices.end();++d)
		delete *d;
}

void Input::update()
{
	DeviceVector::iterator d(_devices.begin());
	for(;d!=_devices.end();++d)
		(*d)->update();
}

