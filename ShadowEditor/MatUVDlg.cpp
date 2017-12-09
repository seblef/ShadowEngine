// MatUVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MatUVDlg.h"
#include "afxdialogex.h"
#include "MaterialPropertySheet.h"


// CMatUVDlg dialog

IMPLEMENT_DYNAMIC(CMatUVDlg, CPropertyPage)

CMatUVDlg::CMatUVDlg(Material *m, CMaterialPropertySheet* ps)
	: CPropertyPage(CMatUVDlg::IDD), _mat(m), _parent(ps), _init(false)
{

}

CMatUVDlg::~CMatUVDlg()
{
}

void CMatUVDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAT_UOFFSP, _uOffset);
	DDX_Control(pDX, IDC_USCALESP, _uScale);
	DDX_Control(pDX, IDC_MAT_VOFFSP, _vOffset);
	DDX_Control(pDX, IDC_MAT_VSCALESP, _vScale);
	DDX_Control(pDX, IDC_MAT_ROTSP, _rot);
}


BEGIN_MESSAGE_MAP(CMatUVDlg, CPropertyPage)
	ON_EN_CHANGE(IDC_MAT_UOFF, &CMatUVDlg::updateValues)
	ON_EN_CHANGE(IDC_MAT_USCALE, &CMatUVDlg::updateValues)
	ON_EN_CHANGE(IDC_MAT_ROT, &CMatUVDlg::updateValues)
	ON_EN_CHANGE(IDC_MAT_VOFF, &CMatUVDlg::updateValues)
	ON_EN_CHANGE(IDC_MAT_VSCALE, &CMatUVDlg::updateValues)
END_MESSAGE_MAP()


// CMatUVDlg message handlers


BOOL CMatUVDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//				UV transforms
	_uOffset.SetRange32(0,1000);					_uOffset.SetPos32(_mat->getStdBuffer().getOffset().x * 100.0f);
	_vOffset.SetRange32(0,1000);					_vOffset.SetPos32(_mat->getStdBuffer().getOffset().y * 100.0f);

	_uScale.SetRange32(1,10000);					_uScale.SetPos32(_mat->getStdBuffer().getScale().x * 100.0f);
	_vScale.SetRange32(1,10000);					_vScale.SetPos32(_mat->getStdBuffer().getScale().y * 100.0f);

	_rot.SetRange32(-360,360);						_rot.SetPos32(acosf(_mat->getStdBuffer().getRotation()(0,0)) * 180.0f / M_PI);

	_init=true;

	return TRUE;
}

void CMatUVDlg::updateValues()
{
	if(!_init)			return;

	Vector2 off,scale;
	float rot;
	Matrix2 mrot;

	off.x=(float)_uOffset.GetPos32();
	off.y=(float)_vOffset.GetPos32();

	scale.x=(float)_uScale.GetPos32();
	scale.y=(float)_vScale.GetPos32();

	rot=(float)_rot.GetPos32();

	off*=0.01f;
	scale*=0.01f;
	mrot.rotation(rot * M_PI /180.0f);

	_mat->getStdBuffer().setOffset(off);
	_mat->getStdBuffer().setScale(scale);
	_mat->getStdBuffer().setRotation(mrot);

	_parent->refresh();
}
