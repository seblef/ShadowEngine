// MeshTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MeshTemplateDlg.h"
#include "MaterialPropertySheet.h"
#include "afxdialogex.h"
#include "G_EditorApp.h"
#include "GameMaterialEditor.h"
#include "../Game/GameObject.h"


// CMeshTemplateDlg dialog

IMPLEMENT_DYNAMIC(CMeshTemplateDlg, CDialogEx)

CMeshTemplateDlg::CMeshTemplateDlg(G_MeshTemplate *t, CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeshTemplateDlg::IDD, pParent), _mesh(t), _save(t),
	_preview(&t->getMaterial(),t->getGeometry(),G_EditorApp::getSingletonRef().getVideo())
	, _name(t->getName().c_str())
	, _geoName(t->getGeometryFile().c_str())
	, _noCollision(t->getFlag(OF_NOCOLLISION))
{

}

CMeshTemplateDlg::~CMeshTemplateDlg()
{
}

void CMeshTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MT_PREVIEW, _preview);
	DDX_Text(pDX, IDC_MT_NAME, _name);
	DDX_Text(pDX, IDC_MT_GEOMETRY, _geoName);
	DDX_Control(pDX, IDC_MT_MAT, _materials);
	DDX_Control(pDX, IDC_MT_SHAPE, _shapes);
	DDX_Check(pDX, IDC_MT_NOCOLLISION, _noCollision);
	DDX_Control(pDX, IDC_MT_GMAT, _gameMaterials);
}


BEGIN_MESSAGE_MAP(CMeshTemplateDlg, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_MT_MAT, &CMeshTemplateDlg::OnCbnSelchangeMtMat)
	ON_BN_CLICKED(IDC_MT_MATEDIT, &CMeshTemplateDlg::OnBnClickedMtMatedit)
	ON_BN_CLICKED(IDOK, &CMeshTemplateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMeshTemplateDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MT_GMATEDIT, &CMeshTemplateDlg::OnBnClickedMtGmatedit)
END_MESSAGE_MAP()


// CMeshTemplateDlg message handlers


BOOL CMeshTemplateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_preview.initDialog();

	refreshMaterialList();
	refreshGameMaterialList();

	for(int i=0;i<PSHAPE_COUNT;++i)
		_shapes.AddString(g_PhysicShapeNames[i]);

	_shapes.SetCurSel(_mesh->getPhysicShape());

	return TRUE;
}

void CMeshTemplateDlg::refreshMaterialList()
{
	_materials.ResetContent();

	const map<string,Material*>& db(MaterialSystem::getSingletonRef().getMaterials().getData());
	map<string,Material*>::const_iterator m(db.begin());

	int sel=0;
	for(;m!=db.end();++m)
		_materials.AddString(m->first.c_str());

	CString n;
	for(int i=0;i<_materials.GetCount();++i)
	{
		_materials.GetLBText(i,n);
		if(_mesh->getMaterialName()==(const char*)n)
			sel=i;
	}

	_materials.SetCurSel(sel);
}

void CMeshTemplateDlg::refreshGameMaterialList()
{
	_gameMaterials.ResetContent();

	const map<string, G_GameMaterial*>& db(G_EditorApp::getSingletonRef().getMap().getGameMaterials().getData());
	map<string, G_GameMaterial*>::const_iterator m(db.begin());

	int sel = 0;
	_gameMaterials.AddString("null");
	for (; m != db.end(); ++m)
		_gameMaterials.AddString(m->first.c_str());

	if (_mesh->getGameMaterial())
	{
		CString n;
		for (int i = 0; i<_gameMaterials.GetCount(); ++i)
		{
			_gameMaterials.GetLBText(i, n);
			if (_mesh->getGameMaterial()->getName() == (const char*)n)
				sel = i;
		}
	}

	_gameMaterials.SetCurSel(sel);

}

void CMeshTemplateDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint p(point);
	ClientToScreen(&p);

	CRect rp;
	_preview.GetWindowRect(&rp);

	if(p.x > rp.left && p.x < rp.right && p.y > rp.top && p.y < rp.bottom)
		_preview.OnLButtonUp(0,p);

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMeshTemplateDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint p(point);
	ClientToScreen(&p);

	CRect rp;
	_preview.GetWindowRect(&rp);

	if(p.x > rp.left && p.x < rp.right && p.y > rp.top && p.y < rp.bottom)
		_preview.OnLButtonDown(0,p);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMeshTemplateDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint p(point);
	ClientToScreen(&p);

	CRect rp;
	_preview.GetWindowRect(&rp);

	if(p.x > rp.left && p.x < rp.right && p.y > rp.top && p.y < rp.bottom)
		_preview.OnMouseMove(0,p);
	else
		_preview.OnLButtonUp(0,p);

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMeshTemplateDlg::OnCbnSelchangeMtMat()
{
	int sel=_materials.GetCurSel();
	if(sel!=-1)
	{
		CString mat;
		_materials.GetLBText(sel,mat);

		_mesh->setMaterial((const char*)mat);
		_preview.setMaterial(MaterialSystem::getSingletonRef().getMaterial((const char*)mat));
	}

	_preview.update(0);
}


void CMeshTemplateDlg::OnBnClickedMtMatedit()
{
	CMaterialPropertySheet dlg(&_mesh->getMaterial(),_mesh->getMaterialName().c_str(),FALSE,"Material");
	dlg.DoModal();

	_preview.update(0);
}


void CMeshTemplateDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	_mesh->setName((const char*)_name);
	_mesh->setPhysicShape((PhysicShape)_shapes.GetCurSel());
	
	if(_noCollision)
		_mesh->setFlag(OF_NOCOLLISION);
	else
		_mesh->unsetFlag(OF_NOCOLLISION);

	int sel = _gameMaterials.GetCurSel();
	if (sel > 0)
	{
		CString gameMat;
		_gameMaterials.GetLBText(sel, gameMat);
		_mesh->setGameMaterial(G_EditorApp::getSingletonRef().getMap().getGameMaterials().getData((const char*)gameMat));
	}
	else
		_mesh->setGameMaterial(0);

	CDialogEx::OnOK();
}


void CMeshTemplateDlg::OnBnClickedCancel()
{
	_save.restore();
	CDialogEx::OnCancel();
}


void CMeshTemplateDlg::OnBnClickedMtGmatedit()
{
	if (_mesh->getGameMaterial())
	{
		string oldName(_mesh->getGameMaterial()->getName());
		CGameMaterialEditor dlg(_mesh->getGameMaterial());
		dlg.DoModal();
		_mesh->getGameMaterial()->setName(oldName);
	}
}
