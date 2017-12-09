
// ShadowEditor.h : main header file for the ShadowEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CShadowEditorApp:
// See ShadowEditor.cpp for the implementation of this class
//

class CShadowEditorApp : public CWinAppEx
{
public:
	CShadowEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CShadowEditorApp theApp;
