// SelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "SelectionDlg.h"
#include "afxdialogex.h"


// CSelectionDlg dialog

IMPLEMENT_DYNAMIC(CSelectionDlg, CDialogEx)

CSelectionDlg::CSelectionDlg(const vector<string>* items, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectionDlg::IDD, pParent)
{
	if(items)
		setItems(*items);
}

CSelectionDlg::~CSelectionDlg()
{
}

void CSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEL_LIST, _list);
}


BEGIN_MESSAGE_MAP(CSelectionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectionDlg message handlers


BOOL CSelectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for(int i=0;i<_items.size();++i)
		_list.AddString(_items[i].c_str());

	_list.SetCurSel(0);

	return TRUE;
}


void CSelectionDlg::OnBnClickedOk()
{
	int sel=_list.GetCurSel();
	if(sel!=-1)
	{
		CString s;
		_list.GetText(sel,s);
		_selected=(const char*)s;
	}

	CDialogEx::OnOK();
}
