#include "stdafx.h"
#include "KeyboardDevice.h"
#include <MediaCommon.h>
#include <assert.h>

KeyboardDevice::KeyboardDevice(IDirectInput8* di, const WinWindow& win)
{
	if(di->CreateDevice(GUID_SysKeyboard,&_device,0)==DI_OK)
	{
		_device->SetDataFormat(&c_dfDIKeyboard);
		_device->SetCooperativeLevel(win.getHWnd(),DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		_device->Acquire();
	}

	memset(_lastKeys,0,256);
}

KeyboardDevice::~KeyboardDevice()
{
	if(_device)			_device->Release();
}

void KeyboardDevice::update()
{
	unsigned char k[256];
	memset(k,0,256);

	_device->Acquire();
	if(_device->GetDeviceState(256,k)==DIERR_INPUTLOST)
		return;

	for(int i=0;i<256;++i)
	{
		if(buttonDown(k[i]) && buttonUp(_lastKeys[i]))
			EventPump::getSingletonRef().postEvent(Event(ET_KEYDOWN,i));
		else if(buttonUp(k[i]) && buttonDown(_lastKeys[i]))
			EventPump::getSingletonRef().postEvent(Event(ET_KEYUP,i));
	}

	memcpy(_lastKeys,k,256);
}
