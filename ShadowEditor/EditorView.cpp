
#include "stdafx.h"
#include "EditorView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "ShadowEditor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CEditorView::CEditorView() : _currentEdit(0)
{
}

CEditorView::~CEditorView()
{
}

BEGIN_MESSAGE_MAP(CEditorView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CEditorView::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	if(_currentEdit)
		_currentEdit->SetWindowPos(NULL,rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	AdjustLayout();

	return 0;
}

void CEditorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CEditorView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	if(_currentEdit)
		_currentEdit->SetFocus();
}

void CEditorView::setEditor(CEditDlg* dialog)
{
	if(_currentEdit)
		delete _currentEdit;

	_currentEdit=dialog;
	dialog->Create(dialog->getIDD(),this);
	dialog->ShowWindow(SW_SHOW);

	AdjustLayout();
}