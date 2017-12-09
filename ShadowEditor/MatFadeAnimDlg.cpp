// MatFadeAnimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MatFadeAnimDlg.h"
#include "afxdialogex.h"


// CMatFadeAnimDlg dialog

IMPLEMENT_DYNAMIC(CMatFadeAnimDlg, CPropertyPage)

CMatFadeAnimDlg::CMatFadeAnimDlg(Material *m, CMaterialPropertySheet* ps)
: CMatAnimDlg(CMatFadeAnimDlg::IDD,m,ps), _init(false)
	, _enable(FALSE)
{

}

CMatFadeAnimDlg::~CMatFadeAnimDlg()
{
}

void CMatFadeAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAT_FADESTARTSP, _start);
	DDX_Control(pDX, IDC_MAT_FADEENDSP, _end);
	DDX_Check(pDX, IDC_MAT_FADEENABLE, _enable);
}


BEGIN_MESSAGE_MAP(CMatFadeAnimDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_MAT_FADEENABLE, &CMatFadeAnimDlg::OnBnClickedMatFadeenable)
	ON_EN_CHANGE(IDC_MAT_FADEEND, &CMatFadeAnimDlg::updateVals)
	ON_EN_CHANGE(IDC_MAT_FADESTART, &CMatFadeAnimDlg::updateVals)
END_MESSAGE_MAP()


// CMatFadeAnimDlg message handlers


BOOL CMatFadeAnimDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	_start.SetRange32(0,10000);
	_end.SetRange32(1,100000);

	enableCtrls(getMaterialAnimation("fade")==-1 ? FALSE : TRUE);

	_init=true;

	return TRUE;
}


void CMatFadeAnimDlg::OnBnClickedMatFadeenable()
{
	UpdateData(TRUE);
	if(_enable)
	{
		_material->createAnimation("fade");
		MaterialFadeAnimation *a=(MaterialFadeAnimation*)_material->getAnimation(getMaterialAnimation("fade"));

		_init=false;
		_start.SetPos32((int)(a->getStartTime() * 1000.0f));
		_end.SetPos32((int)(a->getEndTime() * 1000.0f));
		UpdateData(FALSE);
		_init=true;
	}
	else
	{
		int idx=getMaterialAnimation("fade");
		_material->deleteAnimation(idx);
	}

	enableCtrls(_enable);
}

void CMatFadeAnimDlg::updateVals()
{
	if(!_init)		return;

	UpdateData(TRUE);

	MaterialFadeAnimation *a=(MaterialFadeAnimation*)_material->getAnimation(getMaterialAnimation("fade"));

	a->setStartTime(0.001f * (float)_start.GetPos32());
	a->setEndTime(0.001f * (float)_end.GetPos32());


}

void CMatFadeAnimDlg::enableCtrls(BOOL e)
{
	const UINT ctrls[4]={
		IDC_MAT_FADEEND,
		IDC_MAT_FADEENDSP,
		IDC_MAT_FADESTART,
		IDC_MAT_FADESTARTSP
	};

	for(int i=0;i<4;++i)
		GetDlgItem(ctrls[i])->EnableWindow(e);

	((CButton*)GetDlgItem(IDC_MAT_FADEENABLE))->SetCheck(e ? BST_CHECKED : BST_UNCHECKED);

	if(e)
	{
		MaterialFadeAnimation *a=(MaterialFadeAnimation*)_material->getAnimation(getMaterialAnimation("fade"));

		_start.SetPos32((int)(a->getStartTime() * 1000.0f));
		_end.SetPos32((int)(a->getEndTime() * 1000.0f));
	}

	UpdateData(FALSE);
}
