// NullEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "NullEditDlg.h"
#include "afxdialogex.h"


// CNullEditDlg dialog

IMPLEMENT_DYNAMIC(CNullEditDlg, CDialogEx)

CNullEditDlg::CNullEditDlg(CWnd* pParent /*=NULL*/)
: CEditDlg(CNullEditDlg::IDD, pParent)
{

}

CNullEditDlg::~CNullEditDlg()
{
}

void CNullEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNullEditDlg, CDialogEx)
END_MESSAGE_MAP()


// CNullEditDlg message handlers
