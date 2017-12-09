
// ShadowEditorView.h : interface of the CShadowEditorView class
//

#pragma once

#include <MediaCommon.h>


class CShadowEditorView : public CView
{
protected: // create from serialization only
	CShadowEditorView();
	DECLARE_DYNCREATE(CShadowEditorView)

// Attributes
public:
	CShadowEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CShadowEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	IView*				_view;
	bool				_dragging;
	int					_lastMouseX,_lastMouseY;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseLeave();
};

#ifndef _DEBUG  // debug version in ShadowEditorView.cpp
inline CShadowEditorDoc* CShadowEditorView::GetDocument() const
   { return reinterpret_cast<CShadowEditorDoc*>(m_pDocument); }
#endif

