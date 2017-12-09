
#ifndef _AIDEBUGGER_H_
#define _AIDEBUGGER_H_

#ifdef _DEBUG_AI

#include <Core.h>
#include <Windows.h>

using namespace Core;

class AIDebugger : public TSingleton<AIDebugger>
{
protected:

	HWND			_window;
	HWND			_list;
	HWND			_clearButton;

	ofstream		_txtOutput;

	bool			_closed;

public:

	AIDebugger();
	~AIDebugger();

	void			logOut(const char* msg, ...);
	void			update();

	void			setList(HWND hList)				{ _list = hList; }
	void			setButton(HWND hButton)			{ _clearButton = hButton; }

	HWND			getList() const					{ return _list; }
	HWND			getButton() const				{ return _clearButton; }

	void			setClosed()						{ _closed = true; }
};

#endif

#endif