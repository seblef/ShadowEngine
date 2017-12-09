// GameMaterialEditor.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "GameMaterialEditor.h"
#include "afxdialogex.h"
#include "G_EditorApp.h"


// CGameMaterialEditor dialog

IMPLEMENT_DYNAMIC(CGameMaterialEditor, CDialogEx)

CGameMaterialEditor::CGameMaterialEditor(G_GameMaterial *mat, CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameMaterialEditor::IDD, pParent)
	, _material(mat)
	, _name(mat->getName().c_str())
{

}

CGameMaterialEditor::~CGameMaterialEditor()
{
}

void CGameMaterialEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GMAT_NAME, _name);
	DDX_Control(pDX, IDC_GMAT_DEBRIS, _debris);
}


BEGIN_MESSAGE_MAP(CGameMaterialEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGameMaterialEditor::OnBnClickedOk)
END_MESSAGE_MAP()


// CGameMaterialEditor message handlers


BOOL CGameMaterialEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int sel = 0;
	_debris.AddString("null");

	const TDataBase<G_MeshTemplate>& meshes(G_EditorApp::getSingletonRef().getMap().getMeshes());
	map<string, G_MeshTemplate*>::const_iterator m(meshes.getData().begin());
	int i = 0;
	for (; m != meshes.getData().end(); ++m,++i)
	{
//		if (_material->getDebris() != m->second)		Check game material
		{
			_debris.AddString(m->first.c_str());
			if (m->second == _material->getDebris())
				sel = i;
		}
	}

	_debris.SetCurSel(sel);

	return TRUE;
}


void CGameMaterialEditor::OnBnClickedOk()
{
	UpdateData(TRUE);

	_material->setName((const char*)_name);
	int sel = _debris.GetCurSel();
	if (sel != -1)
	{
		if (sel == 0)
			_material->setDebris(0);
		else
		{
			CString ssel;
			_debris.GetLBText(sel, ssel);
			_material->setDebris(G_EditorApp::getSingletonRef().getMap().getMeshes().getData((const char*)ssel));
		}
	}

	CDialogEx::OnOK();
}
