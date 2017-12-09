// MatTextureAnimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MatTextureAnimDlg.h"
#include "afxdialogex.h"


// CMatTextureAnimDlg dialog

IMPLEMENT_DYNAMIC(CMatTextureAnimDlg, CPropertyPage)

CMatTextureAnimDlg::CMatTextureAnimDlg(Material *m, CMaterialPropertySheet *ps)
: CMatAnimDlg(CMatTextureAnimDlg::IDD, m,ps), _init(false), _enable(FALSE), _time(0)
{

}

CMatTextureAnimDlg::~CMatTextureAnimDlg()
{
}

void CMatTextureAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_MTEX_ENABLE, _enable);
	DDX_Text(pDX, IDC_MTEX_TIME, _time);
	DDX_Control(pDX, IDC_MTEX_TPRSP, _rowSP);
	DDX_Control(pDX, IDC_MTEX_TPCSP, _colSP);
	DDX_Control(pDX, IDC_MTEX_TCSP, _countSP);
}


BEGIN_MESSAGE_MAP(CMatTextureAnimDlg, CPropertyPage)
	ON_EN_CHANGE(IDC_MTEX_TPERROW, &CMatTextureAnimDlg::updateVals)
	ON_EN_CHANGE(IDC_MTEX_TPERCOL, &CMatTextureAnimDlg::updateVals)
	ON_EN_CHANGE(IDC_MTEX_TCOUNT, &CMatTextureAnimDlg::updateVals)
	ON_EN_CHANGE(IDC_MTEX_TIME, &CMatTextureAnimDlg::updateVals)
	ON_BN_CLICKED(IDC_MTEX_ENABLE, &CMatTextureAnimDlg::OnBnClickedMtexEnable)
END_MESSAGE_MAP()


// CMatTextureAnimDlg message handlers


BOOL CMatTextureAnimDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	_rowSP.SetRange32(1,100);
	_colSP.SetRange32(1,100);
	_countSP.SetRange32(1,10000);

	enableCtrls(getMaterialAnimation("texture")==-1 ? FALSE : TRUE);

	_init=true;

	return TRUE;
}

void CMatTextureAnimDlg::updateVals()
{
	if(!_init)		return;

	UpdateData(TRUE);

	MaterialTextureAnimation *a=(MaterialTextureAnimation*)_material->getAnimation(getMaterialAnimation("texture"));

	a->setTilePerRow(_rowSP.GetPos32());
	a->setTilePerColumn(_colSP.GetPos32());
	a->setTileCount(_countSP.GetPos32());
	a->setAnimTime(0.001f * (float)_time);
}

void CMatTextureAnimDlg::OnBnClickedMtexEnable()
{
	UpdateData(TRUE);
	if(_enable)
	{
		_material->createAnimation("texture");
		MaterialTextureAnimation *a=(MaterialTextureAnimation*)_material->getAnimation(getMaterialAnimation("texture"));

		_init=false;
		_rowSP.SetPos32(a->getTilePerRow());
		_colSP.SetPos32(a->getTilePerColumn());
		_countSP.SetPos32(a->getTileCount());
		_time=(UINT)(a->getAnimTime() * 1000.0f);
		UpdateData(FALSE);
		_init=true;
	}
	else
	{
		int idx=getMaterialAnimation("texture");
		_material->deleteAnimation(idx);
	}

	enableCtrls(_enable);
}

void CMatTextureAnimDlg::enableCtrls(BOOL e)
{
	const UINT ctrls[7]={
		IDC_MTEX_TCOUNT,
		IDC_MTEX_TCSP,
		IDC_MTEX_TIME,
		IDC_MTEX_TPCSP,
		IDC_MTEX_TPERCOL,
		IDC_MTEX_TPERROW,
		IDC_MTEX_TPRSP
	};

	for(int i=0;i<7;++i)
		GetDlgItem(ctrls[i])->EnableWindow(e);

	((CButton*)GetDlgItem(IDC_MTEX_ENABLE))->SetCheck(e ? BST_CHECKED : BST_UNCHECKED);

	if(e)
	{
		MaterialTextureAnimation *a=(MaterialTextureAnimation*)_material->getAnimation(getMaterialAnimation("texture"));

		_rowSP.SetPos32(a->getTilePerRow());
		_colSP.SetPos32(a->getTilePerColumn());
		_countSP.SetPos32(a->getTileCount());
		_time=(UINT)(a->getAnimTime() * 1000.0f);
	}

	UpdateData(FALSE);
}