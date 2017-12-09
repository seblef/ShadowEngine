// MatColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MatColorDlg.h"
#include "afxdialogex.h"
#include "G_Utils.h"
#include "MaterialPropertySheet.h"


// CMatColorDlg dialog

IMPLEMENT_DYNAMIC(CMatColorDlg, CPropertyPage)

CMatColorDlg::CMatColorDlg(Material *m, CMaterialPropertySheet* ps, const CString& matName, BOOL editName)
: CPropertyPage(CMatColorDlg::IDD), _mat(m), _parent(ps), _editName(editName)
	, _name(matName)
	, _init(false)
{

}

CMatColorDlg::~CMatColorDlg()
{
}

void CMatColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAT_NAME, _name);
	DDX_Control(pDX, IDC_MAT_AMBIENTCOL, _ambientBtn);
	DDX_Control(pDX, IDC_MAT_DIFFUSECOL, _diffuseBtn);
	DDX_Control(pDX, IDC_MAT_SPECCOL, _specBtn);
	DDX_Control(pDX, IDC_MAT_SHINESP, _shininessSP);
	DDX_Control(pDX, IDC_MAT_SINTSP, _specIntensity);
}


BEGIN_MESSAGE_MAP(CMatColorDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_MAT_AMBIENTCOL, &CMatColorDlg::updateValues)
	ON_BN_CLICKED(IDC_MAT_DIFFUSECOL, &CMatColorDlg::updateValues)
	ON_BN_CLICKED(IDC_MAT_SPECCOL, &CMatColorDlg::updateValues)
	ON_EN_CHANGE(IDC_MAT_SHININESS, &CMatColorDlg::updateValues)
	ON_EN_CHANGE(IDC_MAT_SINTENSITY, &CMatColorDlg::updateValues)
	ON_EN_CHANGE(IDC_MAT_NAME, &CMatColorDlg::OnEnChangeMatName)
END_MESSAGE_MAP()


// CMatColorDlg message handlers


BOOL CMatColorDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_MAT_NAME)->EnableWindow(_editName);

	//				Colors
	_ambientBtn.SetColor(G_Utils::toColorRef(_mat->getStdBuffer().getAmbient()));
	_diffuseBtn.SetColor(G_Utils::toColorRef(_mat->getStdBuffer().getDiffuse()));
	_specBtn.SetColor(G_Utils::toColorRef(_mat->getStdBuffer().getSpecular()));

	_shininessSP.SetRange32(0,10000);		_shininessSP.SetPos32((int)_mat->getStdBuffer().getShininess());
	_specIntensity.SetRange32(0,10000);		_specIntensity.SetPos32((int)(_mat->getStdBuffer().getSpecIntensity()*100.0f));

	UpdateData(FALSE);
	_init=true;

	return TRUE;
}


void CMatColorDlg::updateValues()
{
	if(!_init)				return;

	Color c;

	G_Utils::toColor(_ambientBtn.GetColor(),c);
	_mat->getStdBuffer().setAmbient(c);

	G_Utils::toColor(_diffuseBtn.GetColor(),c);
	c.a=_mat->getStdBuffer().getDiffuse().a;
	_mat->getStdBuffer().setDiffuse(c);

	G_Utils::toColor(_specBtn.GetColor(),c);
	_mat->getStdBuffer().setSpecular(c);

	_mat->getStdBuffer().setShininess((float)_shininessSP.GetPos32());
	_mat->getStdBuffer().setSpecIntensity(0.01f * (float)_specIntensity.GetPos32());

	_parent->refresh();
}

void CMatColorDlg::OnEnChangeMatName()
{
	UpdateData(TRUE);
}
