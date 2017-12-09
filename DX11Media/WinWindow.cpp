
#include "stdafx.h"
#include "WinWindow.h"
#include <MediaCommon.h>

LRESULT CALLBACK WndFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);



WinWindow::WinWindow(HINSTANCE hInst, int width, int height, const string& title) : _hInstance(hInst), _hWnd(0), _width(width), _height(height)
{
	WNDCLASS    wndclass;

	wndclass.style=CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc=WndFunc;
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=_hInstance;
	wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName=NULL;
	wndclass.lpszClassName=title.c_str();

	RegisterClass(&wndclass);

	_hWnd=CreateWindowEx(WS_EX_APPWINDOW,
		title.c_str(),
		title.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE | WS_SYSMENU,
		100,0,
		_width,_height+30,
		NULL,NULL,_hInstance,NULL);              
     
	ShowWindow(_hWnd, TRUE);
	UpdateWindow(_hWnd);
	SetFocus(_hWnd);

	ShowCursor(FALSE);
}

WinWindow::~WinWindow()
{
	if(_hWnd)
		DestroyWindow(_hWnd);
}

void WinWindow::setPos(int x, int y)
{
}

void WinWindow::getPos(int &x, int &y) const
{
	x=0;
	y=0;
}


LRESULT CALLBACK WndFunc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;

	switch(message)
	{
		case WM_PAINT:
			hdc=BeginPaint(hwnd,&ps);
			EndPaint(hwnd,&ps);
			return FALSE;
			break;

		case WM_DESTROY:
			EventPump::getSingletonRef().postEvent(Event(ET_CLOSE));
			break;

		case WM_SYSCOMMAND:
			if((wParam & 0xFFF0)==SC_SCREENSAVE || 
			   (wParam & 0xFFF0)==SC_MONITORPOWER)
				return 0;
			break;

		case WM_SIZE:
			switch(wParam)
			{
				case SIZE_MINIMIZED:
					EventPump::getSingletonRef().postEvent(Event(ET_MINIMIZE));
					break;

				case SIZE_RESTORED:
					EventPump::getSingletonRef().postEvent(Event(ET_RESTORE));
					break;
			}
			break;

		case WM_KILLFOCUS:
			EventPump::getSingletonRef().postEvent(Event(ET_LOSTFOCUS));
			break;

		case WM_SETFOCUS:
			EventPump::getSingletonRef().postEvent(Event(ET_GETFOCUS));
			break;
			
		case WM_SETCURSOR:
			SetCursor(NULL);
			ShowCursor(FALSE);
			break;
 	}

   	return DefWindowProc (hwnd, message, wParam, lParam);
}
