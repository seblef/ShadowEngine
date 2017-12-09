
#ifndef _INPUT_H_
#define _INPUT_H_

#include <MediaCommon.h>
#include "WinWindow.h"
#include "IInputDevice.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>

using namespace std;

class Input
{
protected:

	typedef vector<IInputDevice*> DeviceVector;

	IDirectInput8*			_di;
	DeviceVector			_devices;

public:

	Input(const WinWindow& win, unsigned int flags);
	~Input();

	void					update();
};

#endif