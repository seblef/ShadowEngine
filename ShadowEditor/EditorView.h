
#pragma once

#include "EditDlg.h"

class CEditorView : public CDockablePane
{
// Construction
public:
	CEditorView();

	void AdjustLayout();

// Attributes
public:

	void				setEditor(CEditDlg* dialog);

protected:

	CEditDlg*			_currentEdit;

// Implementation
public:
	virtual ~CEditorView();

	CEditDlg*			getDialog() const				{ return _currentEdit; }

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

