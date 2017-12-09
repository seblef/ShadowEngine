
#ifndef _WINWINDOW_H_
#define _WINWINDOW_H_

#include <windows.h>
#include <string>

using namespace std;

class WinWindow
{
	private:

		HWND					_hWnd;
		HINSTANCE				_hInstance;

		int						_width, _height;

	public:

		WinWindow(HINSTANCE hInst, int width, int height, const string& title);
		~WinWindow();

		void					setPos(int x, int y);
		void					getPos(int& x, int& y) const;

		void					getSize(int& iWidth, int& iHeight) const	{ iWidth=_width; iHeight=_height; }

		HWND					getHWnd() const								{ return _hWnd; }
		HINSTANCE				getHInstance() const						{ return _hInstance; }
};

#endif