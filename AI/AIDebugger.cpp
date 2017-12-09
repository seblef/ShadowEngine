
#include "stdafx.h"
#include "AIDebugger.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef _DEBUG_AI

extern HINSTANCE			g_AppHInstance;

#define ID_AIDEBUG_LIST				1
#define ID_AIDEBUG_CLEAR			2


LRESULT CALLBACK AIDebugWndFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int          iIndex, iLength, cxChar, cyChar;
	HWND hlist, hbutton;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());


		// Create listbox and static text windows.

		hlist = CreateWindow(TEXT("listbox"), NULL,
			WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_HSCROLL | WS_BORDER,
			10, 10,
			370,
			700,
			hwnd, (HMENU)ID_AIDEBUG_LIST,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);

		hbutton = CreateWindow(TEXT("button"), "Clear",
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			10, 710,
			370,50,
			hwnd, (HMENU)ID_AIDEBUG_CLEAR,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);

		AIDebugger::getSingletonRef().setList(hlist);
		AIDebugger::getSingletonRef().setButton(hbutton);

		return 0;
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_AIDEBUG_CLEAR)
		{
			SendMessage(AIDebugger::getSingletonRef().getList(), LB_RESETCONTENT, 0, 0);
		}
		return 0;
		break;

	case WM_DESTROY:
		AIDebugger::getSingletonRef().setClosed();
		break;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


AIDebugger::AIDebugger() : _txtOutput("AI.log"), _closed(false)
{
	WNDCLASS    wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = AIDebugWndFunc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = g_AppHInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "AI Debugger";

	RegisterClass(&wndclass);

	_window = CreateWindowEx(WS_EX_APPWINDOW,
		"AI Debugger",
		"AI Debugger",
		WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE | WS_SYSMENU,
		1000, 100,
		400, 800,
		NULL, NULL, g_AppHInstance, NULL);

	ShowWindow(_window, TRUE);
	UpdateWindow(_window);
}

AIDebugger::~AIDebugger()
{
	DestroyWindow(_window);
}

void AIDebugger::logOut(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	static char szTempBuffer[10240];
	vsprintf_s(szTempBuffer, msg, args);

	if (!_closed)
	{
		SendMessage(_list, LB_ADDSTRING, 0, (LPARAM)szTempBuffer);
		int count = SendMessage(_list, LB_GETCOUNT, 0, 0);
		SendMessage(_list, LB_SETCURSEL, count - 1, 0);
	}

	_txtOutput << szTempBuffer << endl;
}

void AIDebugger::update()
{

}

#endif