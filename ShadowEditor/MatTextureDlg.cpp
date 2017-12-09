// MatTextureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MatTextureDlg.h"
#include "afxdialogex.h"
#include "MaterialPropertySheet.h"

const UINT g_TextureBtn[Material::TS_COUNT]={
	IDC_MAT_DIFFUSET,
	IDC_BMAT_NORMALT,
	IDC_MAT_SPECT,
	IDC_MAT_EMISSIVET,
	IDC_MAT_ENVTT
};

const UINT g_TextureCheck[Material::TS_COUNT]={
	IDC_MAT_DIFFUSECK,
	IDC_MAT_NORMALCK,
	IDC_MAT_SPECCK,
	IDC_MAT_EMISSIVECK,
	IDC_MAT_ENVTCK
};


// CMatTextureDlg dialog

IMPLEMENT_DYNAMIC(CMatTextureDlg, CPropertyPage)

CMatTextureDlg::CMatTextureDlg(Material *m, CMaterialPropertySheet *ps)
	: CPropertyPage(CMatTextureDlg::IDD), _mat(m), _parent(ps)
{

}

CMatTextureDlg::~CMatTextureDlg()
{
}

void CMatTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMatTextureDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_MAT_DIFFUSET, &CMatTextureDlg::OnBnClickedMatDiffuset)
	ON_BN_CLICKED(IDC_BMAT_NORMALT, &CMatTextureDlg::OnBnClickedBmatNormalt)
	ON_BN_CLICKED(IDC_MAT_SPECT, &CMatTextureDlg::OnBnClickedMatSpect)
	ON_BN_CLICKED(IDC_MAT_EMISSIVET, &CMatTextureDlg::OnBnClickedMatEmissivet)
	ON_BN_CLICKED(IDC_MAT_ENVTT, &CMatTextureDlg::OnBnClickedMatEnvtt)
	ON_BN_CLICKED(IDC_MAT_DIFFUSECK, &CMatTextureDlg::OnBnClickedMatDiffuseck)
	ON_BN_CLICKED(IDC_MAT_NORMALCK, &CMatTextureDlg::OnBnClickedMatNormalck)
	ON_BN_CLICKED(IDC_MAT_SPECCK, &CMatTextureDlg::OnBnClickedMatSpecck)
	ON_BN_CLICKED(IDC_MAT_EMISSIVECK, &CMatTextureDlg::OnBnClickedMatEmissiveck)
	ON_BN_CLICKED(IDC_MAT_ENVTCK, &CMatTextureDlg::OnBnClickedMatEnvtck)
END_MESSAGE_MAP()


// CMatTextureDlg message handlers


BOOL CMatTextureDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//				Textures
	for(int i=0;i<Material::TS_COUNT;++i)
	{
		if(_mat->isDefaultTexture((Material::TextureSlot)i))
			GetDlgItem(g_TextureCheck[i])->EnableWindow(FALSE);
		else
		{
			GetDlgItem(g_TextureBtn[i])->SetWindowText(_mat->getTexture((Material::TextureSlot)i)->getName().c_str());
			((CButton*)GetDlgItem(g_TextureCheck[i]))->SetCheck(BST_CHECKED);
		}
	}

	return TRUE;
}

void CMatTextureDlg::onTextureSelBtn(Material::TextureSlot ts)
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE,_T("*.*"),0,OFN_EXPLORER,_T("Textures (*.*)|*.*||"));

	if(file_dlg.DoModal()==IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());
	string shadowName;

	FileSystemFactory::getFileSystem()->extractLocalFileName(fullName,shadowName);
	_mat->setTexture(ts,shadowName);

	GetDlgItem(g_TextureBtn[ts])->SetWindowText(shadowName.c_str());
	GetDlgItem(g_TextureCheck[ts])->EnableWindow(TRUE);

	((CButton*)GetDlgItem(g_TextureCheck[ts]))->SetCheck(BST_CHECKED);	

	_parent->refresh();
}

void CMatTextureDlg::onTextureCheckBtn(Material::TextureSlot ts)
{
	GetDlgItem(g_TextureBtn[ts])->SetWindowText("<None>");
	GetDlgItem(g_TextureCheck[ts])->EnableWindow(FALSE);

	_mat->setTexture(ts,"");
	_parent->refresh();
}