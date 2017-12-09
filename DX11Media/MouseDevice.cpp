
#include "stdafx.h"
#include "MouseDevice.h"
#include <MediaCommon.h>
#include <assert.h>

MouseDevice::MouseDevice(IDirectInput8 *di, const WinWindow& win)
{
	if(di->CreateDevice(GUID_SysMouse,&_device,0)==DI_OK)
	{
		_device->SetDataFormat(&c_dfDIMouse);
		_device->SetCooperativeLevel(win.getHWnd(),DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		HANDLE hevent=CreateEvent(0,0,0,0);
		_device->SetEventNotification(hevent);

		DIPROPDWORD dipdw={	{ sizeof(DIPROPDWORD), sizeof(DIPROPHEADER), 0, DIPH_DEVICE, }, 32 };
		_device->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph);
		_device->Acquire();
	}

	memset(&_lastState,0,sizeof(_lastState));
}

MouseDevice::~MouseDevice()
{
	if(_device)		_device->Release();
}

void MouseDevice::update()
{
	DIMOUSESTATE mstate;
	memset(&mstate,0,sizeof(mstate));

	_device->Acquire();
	if(_device->GetDeviceState(sizeof(mstate),&mstate)==DIERR_INPUTLOST)
		return;

	if(mstate.lX || mstate.lY)		EventPump::getSingletonRef().postEvent(Event(ET_MOUSEMOVE,(int)mstate.lX,(int)mstate.lY));
	if(mstate.lZ)					EventPump::getSingletonRef().postEvent(Event(ET_MOUSEWHEEL,(int)mstate.lZ));

	for(int i=0;i<3;++i)
	{
		if(buttonDown(mstate.rgbButtons[i]) && buttonUp(_lastState.rgbButtons[i]))
			EventPump::getSingletonRef().postEvent(Event(ET_MOUSEBUTTONDOWN,i));
		else if(buttonUp(mstate.rgbButtons[i]) && buttonDown(_lastState.rgbButtons[i]))
			EventPump::getSingletonRef().postEvent(Event(ET_MOUSEBUTTONUP,i));
	}

	memcpy(&_lastState,&mstate,sizeof(_lastState));
}