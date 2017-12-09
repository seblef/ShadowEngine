// GroundEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "GroundEditDlg.h"
#include "afxdialogex.h"
#include <StdRenderer.h>
#include "G_ToolSystem.h"
#include "G_EditorApp.h"

// CGroundEditDlg dialog

IMPLEMENT_DYNAMIC(CGroundEditDlg, CDialogEx)

CGroundEditDlg::CGroundEditDlg(CWnd* pParent /*=NULL*/)
: CEditDlg(CGroundEditDlg::IDD, pParent)
, _painting(FALSE)
, _erasing(FALSE)
, _settingStartPos(FALSE)
, _wSet(TRUE)
, _wPoint(TRUE)
, _wPaint(FALSE)
{

}

CGroundEditDlg::~CGroundEditDlg()
{
}

void CGroundEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUND_MAT, _materials);
	DDX_Check(pDX, IDC_GROUND_PAINT, _painting);
	DDX_Check(pDX, IDC_GROUND_ERASE, _erasing);
	DDX_Check(pDX, IDC_GROUND_SPSET, _settingStartPos);
	DDX_Check(pDX, IDC_GROUND_WSET, _wSet);
	DDX_Check(pDX, IDC_GROUND_WPOINT, _wPoint);
	DDX_Check(pDX, IDC_GROUND_WPAINT, _wPaint);
	DDX_Control(pDX, IDC_GROUND_GMAT, _gameMaterials);
}


BEGIN_MESSAGE_MAP(CGroundEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GROUND_PAINT, &CGroundEditDlg::OnBnClickedGroundPaint)
	ON_BN_CLICKED(IDC_GROUND_ERASE, &CGroundEditDlg::OnBnClickedGroundErase)
	ON_CBN_SELCHANGE(IDC_GROUND_MAT, &CGroundEditDlg::OnCbnSelchangeGroundMat)
	ON_BN_CLICKED(IDC_GROUND_REFRESHMATS, &CGroundEditDlg::OnBnClickedGroundRefreshmats)
	ON_BN_CLICKED(IDC_GROUND_SPSET, &CGroundEditDlg::OnBnClickedGroundSpset)
	ON_BN_CLICKED(IDC_GROUND_WSET, &CGroundEditDlg::OnBnClickedGroundWset)
	ON_BN_CLICKED(IDC_GROUND_WPOINT, &CGroundEditDlg::OnBnClickedGroundWpoint)
	ON_BN_CLICKED(IDC_GROUND_WPAINT, &CGroundEditDlg::OnBnClickedGroundWpaint)
	ON_CBN_SELCHANGE(IDC_GROUND_GMAT, &CGroundEditDlg::OnCbnSelchangeGroundGmat)
END_MESSAGE_MAP()


// CGroundEditDlg message handlers


void CGroundEditDlg::OnBnClickedGroundPaint()
{
	UpdateData(TRUE);

	const BOOL *others[3]={
		&_erasing,
		&_settingStartPos,
		&_wPaint
	};

	const UINT ids[3]={
		IDC_GROUND_ERASE,
		IDC_GROUND_SPSET,
		IDC_GROUND_WPAINT
	};

	for(int i=0;i<3;++i)
	{
		if(*others[i])
			((CButton*)GetDlgItem(ids[i]))->SetCheck(BST_UNCHECKED);
	}

	if(_painting)
	{
		OnCbnSelchangeGroundMat();
		OnCbnSelchangeGroundGmat();
		G_ToolSystem::getSingletonRef().setCurrentTool(&_tool);
	}
	else
		G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}


void CGroundEditDlg::OnBnClickedGroundErase()
{
	UpdateData(TRUE);

	const BOOL *others[3]={
		&_painting,
		&_settingStartPos,
		&_wPaint
	};

	const UINT ids[3]={
		IDC_GROUND_PAINT,
		IDC_GROUND_SPSET,
		IDC_GROUND_WPAINT
	};

	for(int i=0;i<3;++i)
	{
		if(*others[i])
			((CButton*)GetDlgItem(ids[i]))->SetCheck(BST_UNCHECKED);
	}

	if(_erasing)
	{
		_tool.setMaterial(0);
		_tool.setGameMaterial(0);
		G_ToolSystem::getSingletonRef().setCurrentTool(&_tool);
	}
	else
		G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}


BOOL CGroundEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	OnBnClickedGroundRefreshmats();

	return TRUE;
}


void CGroundEditDlg::OnCbnSelchangeGroundMat()
{
	int sel=_materials.GetCurSel();
	if(sel==-1)
		_tool.setMaterial(0);
	else
	{
		CString mat;
		_materials.GetLBText(sel,mat);
		_tool.setMaterial(MaterialSystem::getSingletonRef().getMaterial(string((const char*)mat)));
	}
}


void CGroundEditDlg::OnBnClickedGroundRefreshmats()
{
	_materials.ResetContent();

	TDataBase<Material>& db(MaterialSystem::getSingletonRef().getMaterials());
	map<string,Material*>::const_iterator m(db.getData().begin());
	for(;m!=db.getData().end();++m)
		_materials.AddString(m->first.c_str());

	_materials.SetCurSel(0);

	_gameMaterials.ResetContent();
	TDataBase<G_GameMaterial>& gdb(G_EditorApp::getSingletonRef().getMap().getGameMaterials());
	map<string, G_GameMaterial*>::const_iterator gm(gdb.getData().begin());
	for (; gm != gdb.getData().end(); ++gm)
		_gameMaterials.AddString(gm->first.c_str());

	_gameMaterials.SetCurSel(0);
}

void CGroundEditDlg::onToolChange()
{
	const BOOL *others[4]={
		&_painting,
		&_erasing,
		&_settingStartPos,
		&_wPaint
	};

	const UINT ids[4]={
		IDC_GROUND_PAINT,
		IDC_GROUND_ERASE,
		IDC_GROUND_SPSET,
		IDC_GROUND_WPAINT
	};

	for(int i=0;i<4;++i)
	{
		if(*others[i])
			((CButton*)GetDlgItem(ids[i]))->SetCheck(BST_UNCHECKED);
	}
}

void CGroundEditDlg::OnBnClickedGroundSpset()
{
	UpdateData(TRUE);

	const BOOL *others[3]={
		&_erasing,
		&_painting,
		&_wPaint
	};

	const UINT ids[3]={
		IDC_GROUND_ERASE,
		IDC_GROUND_PAINT,
		IDC_GROUND_WPAINT
	};

	for(int i=0;i<3;++i)
	{
		if(*others[i])
			((CButton*)GetDlgItem(ids[i]))->SetCheck(BST_UNCHECKED);
	}

	if(_settingStartPos)
	{
		G_ToolSystem::getSingletonRef().setCurrentTool(&_startPosTool);
		G_EditorApp::getSingletonRef().refresh();
	}
	else
		G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}


void CGroundEditDlg::OnBnClickedGroundWset()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_GROUND_WSET)->SetWindowText(_wSet ? "Set" : "Unset");
}


void CGroundEditDlg::OnBnClickedGroundWpoint()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_GROUND_WPOINT)->SetWindowText(_wPoint ? "Point" : "Rect");
}


void CGroundEditDlg::OnBnClickedGroundWpaint()
{
	UpdateData(TRUE);

	const BOOL *others[3]={
		&_erasing,
		&_painting,
		&_settingStartPos
	};

	const UINT ids[3]={
		IDC_GROUND_ERASE,
		IDC_GROUND_PAINT,
		IDC_GROUND_SPSET
	};

	for(int i=0;i<3;++i)
	{
		if(*others[i])
			((CButton*)GetDlgItem(ids[i]))->SetCheck(BST_UNCHECKED);
	}

	if(_wPaint)
	{
		_wakable.setPoint(_wPoint ? true : false);
		_wakable.setWakable(_wSet ? true : false);

		G_ToolSystem::getSingletonRef().setCurrentTool(&_wakable);
		G_EditorApp::getSingletonRef().refresh();
	}
	else
		G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}


void CGroundEditDlg::OnCbnSelchangeGroundGmat()
{
	int sel = _gameMaterials.GetCurSel();
	if (sel == -1)
		_tool.setGameMaterial(0);
	else
	{
		CString mat;
		_gameMaterials.GetLBText(sel, mat);
		_tool.setGameMaterial(G_EditorApp::getSingletonRef().getMap().getGameMaterials().getData((const char*)mat));
	}
}
