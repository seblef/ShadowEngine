// ParticleMaterial.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "ParticleMaterial.h"
#include "afxdialogex.h"
#include "G_EditorApp.h"


// CParticleMaterial dialog

IMPLEMENT_DYNAMIC(CParticleMaterial, CDialogEx)

CParticleMaterial::CParticleMaterial(ParticleMaterial* m, CWnd* pParent /*=NULL*/)
	: CDialogEx(CParticleMaterial::IDD, pParent), _material(m), _changed(false)
	, _blendEnable(FALSE)
	, _textureType(0)
{

}

CParticleMaterial::~CParticleMaterial()
{
}

void CParticleMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PSM_TEXLIST, _setList);
	DDX_Check(pDX, IDC_PSM_BLEND, _blendEnable);
	DDX_Control(pDX, IDC_PSM_SRC, _srcBlend);
	DDX_Control(pDX, IDC_PSM_DEST, _destBlend);
	DDX_Radio(pDX, IDC_PSM_SINGLE, _textureType);
}


BEGIN_MESSAGE_MAP(CParticleMaterial, CDialogEx)
	ON_BN_CLICKED(IDC_PSM_SINGLE, &CParticleMaterial::OnBnClickedPsmSingle)
	ON_BN_CLICKED(IDC_PSM_TSET, &CParticleMaterial::OnBnClickedPsmSingle)
	ON_BN_CLICKED(IDC_PSM_TEX, &CParticleMaterial::OnBnClickedPsmTex)
	ON_BN_CLICKED(IDC_PSM_ADD, &CParticleMaterial::OnBnClickedPsmAdd)
	ON_BN_CLICKED(IDC_PSM_REM, &CParticleMaterial::OnBnClickedPsmRem)
	ON_BN_CLICKED(IDC_PSM_UP, &CParticleMaterial::OnBnClickedPsmUp)
	ON_BN_CLICKED(IDC_PSM_DOWN, &CParticleMaterial::OnBnClickedPsmDown)
	ON_BN_CLICKED(IDC_PSM_BLEND, &CParticleMaterial::OnMaterialChanged)
	ON_CBN_SELCHANGE(IDC_PSM_SRC, &CParticleMaterial::OnMaterialChanged)
	ON_CBN_SELCHANGE(IDC_PSM_DEST, &CParticleMaterial::OnMaterialChanged)
	ON_BN_CLICKED(IDOK, &CParticleMaterial::OnBnClickedOk)
END_MESSAGE_MAP()


// CParticleMaterial message handlers


BOOL CParticleMaterial::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (_material->getTextureSet())
	{
		ParticleTextureSet *s = _material->getTextureSet();
		for (int i = 0; i < s->getTextureCount(); ++i)
			_setList.AddString(s->getTexture(i)->getName().c_str());

		if (s->getTextureCount() > 0)
			_setList.SetCurSel(0);

		_textureType = 1;
		enableSetCtrls(TRUE);
	}
	else
	{
		_textureType = 0;
		enableSetCtrls(FALSE);
		if (_material->getTexture())
			GetDlgItem(IDC_PSM_TEX)->SetWindowText(_material->getTexture()->getName().c_str());
	}

	for (int i = 0; i < BLEND_COUNT; ++i)
	{
		_srcBlend.AddString(g_BlendModeNames[i]);
		_destBlend.AddString(g_BlendModeNames[i]);
	}

	bool blend;
	BlendMode src, dest;

	G_EditorApp::getSingletonRef().getVideo()->getBlendStateDesc(
		_material->getBlendState(), blend, src, dest);

	_blendEnable = blend ? TRUE : FALSE;
	_srcBlend.SetCurSel(src);
	_destBlend.SetCurSel(dest);

	UpdateData(FALSE);
	return TRUE;
}

void CParticleMaterial::enableSetCtrls(BOOL enable)
{
	GetDlgItem(IDC_PSM_TEXLIST)->EnableWindow(enable);
	GetDlgItem(IDC_PSM_ADD)->EnableWindow(enable);
	GetDlgItem(IDC_PSM_REM)->EnableWindow(enable);
	GetDlgItem(IDC_PSM_UP)->EnableWindow(enable);
	GetDlgItem(IDC_PSM_DOWN)->EnableWindow(enable);

	GetDlgItem(IDC_PSM_TEX)->EnableWindow(!enable);
}


void CParticleMaterial::OnBnClickedPsmSingle()
{
	UpdateData(TRUE);
	enableSetCtrls(_textureType == 0 ? FALSE : TRUE);
}

bool CParticleMaterial::selectTexture(string& texName)
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE, _T("*.*"), 0, OFN_EXPLORER, _T("Textures (*.*)|*.*||"));

	if (file_dlg.DoModal() == IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return false;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());

	FileSystemFactory::getFileSystem()->extractLocalFileName(fullName, texName);
	return true;
}


void CParticleMaterial::OnBnClickedPsmTex()
{
	string texName;
	if (selectTexture(texName))
	{
		GetDlgItem(IDC_PSM_TEX)->SetWindowText(texName.c_str());
		_changed = true;
	}
}


void CParticleMaterial::OnBnClickedPsmAdd()
{
	string texName;
	if (selectTexture(texName))
	{
		_setList.AddString(texName.c_str());
		_changed = true;
	}
}


void CParticleMaterial::OnBnClickedPsmRem()
{
	int sel = _setList.GetCurSel();
	if (sel != -1)
	{
		_setList.DeleteString(sel);
		_changed = true;
	}
}


void CParticleMaterial::OnBnClickedPsmUp()
{
	int sel = _setList.GetCurSel();
	if (sel > 0)
	{
		CString strItem;
		_setList.GetText(sel, strItem);
		_setList.DeleteString(sel);
		_setList.InsertString(sel - 1, strItem);
		_setList.SetCurSel(sel - 1);
		_changed = true;
	}
}


void CParticleMaterial::OnBnClickedPsmDown()
{
	int sel = _setList.GetCurSel();
	if (sel!=-1 && sel < _setList.GetCount())
	{
		CString strItem;
		_setList.GetText(sel, strItem);
		_setList.DeleteString(sel);
		_setList.InsertString(sel +1, strItem);
		_setList.SetCurSel(sel + 1);
		_changed = true;
	}
}

ParticleMaterial* CParticleMaterial::createNewMaterial()
{
	BlendMode src=BLEND_SRCALPHA, dest=BLEND_INVSRCALPHA;
	bool blend = _blendEnable ? true : false;
	int sel_src = _srcBlend.GetCurSel();
	int sel_dest = _destBlend.GetCurSel();
	CString strTemp;

	if (sel_src != -1)			src = (BlendMode)sel_src;
	if (sel_dest != -1)			dest = (BlendMode)sel_dest;

	if (_textureType == 0)
	{
		GetDlgItem(IDC_PSM_TEX)->GetWindowText(strTemp);
		string texName((const char *)strTemp);

		if (texName == "<Texture>")
			texName.clear();

		return new ParticleMaterial(texName, false, src, dest);
	}
	else
	{
		vector<string> texs;
		for (int i = 0; i < _setList.GetCount(); ++i)
		{
			_setList.GetText(i, strTemp);
			texs.push_back(string((const char*)strTemp));

		}

		return new ParticleMaterial(texs, src, dest);
	}
}

void CParticleMaterial::OnBnClickedOk()
{
	UpdateData(TRUE);
	_material = createNewMaterial();
	CDialogEx::OnOK();
}
