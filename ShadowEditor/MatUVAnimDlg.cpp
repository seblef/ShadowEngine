// MatUVAnimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MatUVAnimDlg.h"
#include "afxdialogex.h"


// CMatUVAnimDlg dialog

IMPLEMENT_DYNAMIC(CMatUVAnimDlg, CPropertyPage)

CMatUVAnimDlg::CMatUVAnimDlg(Material *m, CMaterialPropertySheet *ps)
	: CMatAnimDlg(IDD,m,ps), _init(false)
{

}

CMatUVAnimDlg::~CMatUVAnimDlg()
{
}

void CMatUVAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_MUV_ENABLE, _enable);
	DDX_Control(pDX, IDC_MUV_UOFFSP, _uOffSP);
	DDX_Control(pDX, IDC_MUV_VOFFSP, _vOffSP);
	DDX_Control(pDX, IDC_MUV_ROTSP, _rotSP);
}


BEGIN_MESSAGE_MAP(CMatUVAnimDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_MUV_ENABLE, &CMatUVAnimDlg::OnBnClickedMuvEnable)
	ON_EN_CHANGE(IDC_MUV_OFF, &CMatUVAnimDlg::updateVals)
	ON_EN_CHANGE(IDC_MUV_VOFF, &CMatUVAnimDlg::updateVals)
	ON_EN_CHANGE(IDC_MUV_ROT, &CMatUVAnimDlg::updateVals)
END_MESSAGE_MAP()


// CMatUVAnimDlg message handlers


BOOL CMatUVAnimDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	_uOffSP.SetRange32(-10000,10000);
	_vOffSP.SetRange32(-10000,10000);
	_rotSP.SetRange32(-3600,3600);

	enableCtrls(getMaterialAnimation("uv")==-1 ? FALSE : TRUE);

	_init=true;

	return TRUE;
}

void CMatUVAnimDlg::OnBnClickedMuvEnable()
{
	UpdateData(TRUE);
	if(_enable)
	{
		_material->createAnimation("uv");
		MaterialUVAnimation *a=(MaterialUVAnimation*)_material->getAnimation(getMaterialAnimation("uv"));

		_init=false;
		_uOffSP.SetPos32((int)(a->getOffset().x * 100.0f));
		_vOffSP.SetPos32((int)(a->getOffset().y * 100.0f));
		_rotSP.SetPos32((int)(a->getRotation()));
		_init=true;
	}
	else
	{
		int idx=getMaterialAnimation("uv");
		_material->deleteAnimation(idx);
	}

	enableCtrls(_enable);
}


void CMatUVAnimDlg::updateVals()
{
	if(!_init)		return;

	UpdateData(TRUE);

	MaterialUVAnimation *a=(MaterialUVAnimation*)_material->getAnimation(getMaterialAnimation("uv"));
	
	Vector2 o;
	o.x=0.01f * (float)_uOffSP.GetPos32();
	o.y=0.01f * (float)_vOffSP.GetPos32();

	a->setOffset(o);
	a->setRotation((float)_rotSP.GetPos32());

	OnPaint();
}

void CMatUVAnimDlg::enableCtrls(BOOL e)
{
	const UINT ctrls[6]={
		IDC_MUV_OFF,
		IDC_MUV_ROT,
		IDC_MUV_ROTSP,
		IDC_MUV_UOFFSP,
		IDC_MUV_VOFF,
		IDC_MUV_VOFFSP
	};

	for(int i=0;i<6;++i)
		GetDlgItem(ctrls[i])->EnableWindow(e);

	((CButton*)GetDlgItem(IDC_MUV_ENABLE))->SetCheck(e ? BST_CHECKED : BST_UNCHECKED);

	if(e)
	{
		MaterialUVAnimation *a=(MaterialUVAnimation*)_material->getAnimation(getMaterialAnimation("uv"));
		_uOffSP.SetPos32((int)(a->getOffset().x * 100.0f));
		_vOffSP.SetPos32((int)(a->getOffset().y * 100.0f));
		_rotSP.SetPos32((int)(a->getRotation()));
	}
}