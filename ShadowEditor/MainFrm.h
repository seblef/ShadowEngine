
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "MainToolBar.h"
#include "ResourceView.h"
#include "EditorView.h"
#include "OutputWnd.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs)						{ return CFrameWndEx::PreCreateWindow(cs); }
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMainToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CResourceView				_resView;
	COutputWnd        m_wndOutput;
	CEditorView					_editorView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnFileDelete();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnToolAlign();
	afx_msg void OnToolCamrotate();
	afx_msg void OnToolCamtranslate();
	afx_msg void OnToolFall2();
	afx_msg void OnToolLockx();
	afx_msg void OnToolLocky();
	afx_msg void OnToolLockz();
	afx_msg void OnToolRedo();
	afx_msg void OnToolRotate();
	afx_msg void OnToolSelect();
	afx_msg void OnToolTranslate();
	afx_msg void OnToolUndo();
	afx_msg void OnEditMapproperties();

	void					onSelectionChange();
	afx_msg void OnCreateMesh();
	afx_msg void OnUpdateToolLockx(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolLocky(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolLockz(CCmdUI *pCmdUI);
	afx_msg void OnToolsSnap();
	afx_msg void OnUpdateToolsSnap(CCmdUI *pCmdUI);
	afx_msg void OnToolsHeight();
	afx_msg void OnCreateLightOmni();
	afx_msg void OnCreateLightSpot();
	afx_msg void OnCreateLightArea();
	afx_msg void OnToolsRotatez();
};


