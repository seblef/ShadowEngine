
#include "stdafx.h"
#include "mainfrm.h"
#include "ResourceView.h"
#include "Resource.h"
#include "MaterialPropertySheet.h"
#include "MeshTemplateDlg.h"
#include "ParticlesEditor.h"
#include "SoundEditor.h"
#include "GameMaterialEditor.h"
#include "G_EditorApp.h"
#include <sstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CFileView

CResourceView::CResourceView()
{
}

CResourceView::~CResourceView()
{
}

BEGIN_MESSAGE_MAP(CResourceView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_RES_NEW, &CResourceView::OnResNew)
	ON_COMMAND(ID_RES_OPEN, &CResourceView::OnResOpen)
	ON_COMMAND(ID_RES_EDIT, &CResourceView::OnResEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CWorkspaceBar

int CResourceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Créer un affichage :
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!_view.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Impossible de créer l'affichage des fichiers\n");
		return -1;      // échec de la création
	}

	// Charger les images de l'affichage :
	_images.Create(IDB_RESOURCES, 16, 0, RGB(255, 0, 255));
	_view.SetImageList(&_images, TVSIL_NORMAL);

	// Load images:
	_toolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_RESOURCES);
	_toolBar.LoadToolBar(IDR_RESOURCES, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	_toolBar.SetPaneStyle(_toolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	_toolBar.SetPaneStyle(_toolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	_toolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	_toolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	refreshResources();

	return 0;
}

void CResourceView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CResourceView::OnContextMenu(CWnd* pWnd, CPoint point)
{
/*	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndNodeView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Sélectionner l'élément activé :
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);*/
}

void CResourceView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = _toolBar.CalcFixedLayout(FALSE, TRUE).cy;

	_toolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	_view.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CResourceView::OnPaint()
{
	CPaintDC dc(this); // contexte de périphérique pour la peinture

	CRect rectTree;
	_view.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CResourceView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_view.SetFocus();
}

void CResourceView::OnChangeVisualStyle()
{
	_images.DeleteImageList();

	UINT uiBmpId = IDB_RESOURCES;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Impossible de charger l'image bitmap : %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK | ILC_COLOR24;

	_images.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	_images.Add(&bmp, RGB(255, 0, 255));

	_view.SetImageList(&_images, TVSIL_NORMAL);
}

void CResourceView::OnResNew()
{
	HTREEITEM hSel=_view.GetSelectedItem();
	if(hSel)
	{
		int img,selimg;
		_view.GetItemImage(hSel,img,selimg);

		if(img==RI_MATERIAL)				newMaterial();
		else if(img==RI_MESH)				newMesh();
		else if (img == RI_PARTICLES)		newParticleSystem();
		else if (img == RI_SOUND)			newSound();
		else if (img == RI_GAMEMATERIAL)	newGameMaterial();
	}
}

void CResourceView::OnResOpen()
{
	HTREEITEM hSel = _view.GetSelectedItem();
	if (hSel)
	{
		int img, selimg;
		_view.GetItemImage(hSel, img, selimg);

		if (img == RI_MATERIAL)				openMaterial();
		else if (img == RI_PARTICLES)		openParticleSystem();
	}
}

void CResourceView::openMaterial()
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE, _T("*.txt"), 0, OFN_EXPLORER, _T("Material (*.txt)|*.txt||"));

	if (file_dlg.DoModal() == IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());
	string shadowName;
	string matName;

	FileSystemFactory::getFileSystem()->extractLocalFileName(fullName, shadowName);
	matName = shadowName.substr(10, shadowName.length() - 14);

	if (MaterialSystem::getSingletonRef().getMaterial(matName) != 0)
		return;

	Material *m = MaterialSystem::getSingletonRef().loadMaterial(matName);
	MaterialSystem::getSingletonRef().registerMaterial(m, matName);

	if (m)		addItem((DWORD_PTR)m, matName, RI_MATERIAL,_materials,true);
}

void CResourceView::openParticleSystem()
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE, _T("*.txt"), 0, OFN_EXPLORER, _T("Particle system (*.txt)|*.txt||"));

	if (file_dlg.DoModal() == IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());
	string shadowName;

	FileSystemFactory::getFileSystem()->extractLocalFileName(fullName, shadowName);

	ScriptFile sf(shadowName);
	if (!sf.good())
		return;

	string token(sf.getToken());
	if (token != "particle_system")
		return;

	string name;
	findBestParticleName(sf.getToken(),name);
	ParticleSystemTemplate *pst = new ParticleSystemTemplate(sf);

	G_ParticlesTemplate *gpt = new G_ParticlesTemplate(pst, name);
	addItem((DWORD_PTR)gpt, gpt->getName(), RI_PARTICLES, _particleSystems, true);
}

void CResourceView::OnResEdit()
{
	HTREEITEM hSel=_view.GetSelectedItem();
	if(hSel!=0 && hSel!=_materials && hSel!=_meshes && hSel!=_particleSystems && hSel!=_sounds)
	{
		int img,selimg;

		CString name(_view.GetItemText(hSel));
		_view.GetItemImage(hSel,img,selimg);

		if(img==RI_MATERIAL)
		{
			Material *m=(Material*)_view.GetItemData(hSel);
			CMaterialPropertySheet dlg(m,name,FALSE,"Material");
			dlg.DoModal();

			if (dlg.getName() != name)
				_view.SetItemText(hSel, dlg.getName());
		}
		else if(img==RI_MESH)
		{
			G_MeshTemplate* t=(G_MeshTemplate*)_view.GetItemData(hSel);
			CMeshTemplateDlg dlg(t);
			dlg.DoModal();

			if (dlg.getName() != name)
			{
				string newName;
				findBestMeshName((const char*)dlg.getName(), newName);
				t->setName(newName);
				_view.SetItemText(hSel, newName.c_str());
			}
		}
		else if (img == RI_PARTICLES)
		{
			G_ParticlesTemplate *sys = (G_ParticlesTemplate*)_view.GetItemData(hSel);
			CParticlesEditor ed(&sys->getTemplate(),sys->getName());
			if (ed.DoModal() == IDOK && ed.getNewTemplate())
			{
				sys->setNewTemplate(ed.getNewTemplate());
				if (ed.getName() != sys->getName())
				{
					string newName;
					findBestParticleName(ed.getName(), newName);
					_view.SetItemText(hSel, newName.c_str());
					sys->setName(newName);
				}
			}
		}
		else if (img == RI_SOUND)
		{
			G_SoundTemplate *t = (G_SoundTemplate*)_view.GetItemData(hSel);
			string oldName(t->getName());
			CSoundEditor ed(t);
			if (ed.DoModal() == IDOK)
			{
				if (t->getName() != oldName)
				{
					string newName;
					findBestSoundName(t->getName(), newName);
					_view.SetItemText(hSel, newName.c_str());
					t->setName(newName);
				}
			}
		}
		else if (img == RI_GAMEMATERIAL)
		{
			G_GameMaterial* m = (G_GameMaterial*)_view.GetItemData(hSel);
			string oldName(m->getName());
			CGameMaterialEditor dlg(m);
			if (dlg.DoModal() == IDOK)
			{
				if (m->getName() != oldName)
				{
					string newName;
					findBestGameMaterialName(m->getName(), newName);
					_view.SetItemText(hSel, newName.c_str());
					m->setName(newName);
				}
			}
		}
	}
}

void CResourceView::newMaterial()
{
	string matName;
	findBestMaterialName("Material", matName);

	Material *m=new Material(G_EditorApp::getSingletonRef().getVideo());
	CMaterialPropertySheet dlg(m,matName.c_str(),TRUE,"Material");
	if(dlg.DoModal()==IDCANCEL)
	{
		delete m;
		return;
	}

	MaterialSystem::getSingletonRef().registerMaterial(m,(const char*)dlg.getName());
	addItem((DWORD_PTR)m, (const char*)dlg.getName(), RI_MATERIAL, _materials,true);
}

void CResourceView::newMesh()
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE,_T("*.*"),0,OFN_EXPLORER,_T("Shadow geometry (*.geo)|*.geo|3DS File (*.3ds)|*.3ds||"));

	if(file_dlg.DoModal()==IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());
	string shadowName;
	string geoName;

	if(!FileSystemFactory::getFileSystem()->extractLocalFileName(fullName,shadowName))
		return;

	geoName=shadowName.substr(9,shadowName.length()-9);

	string meshName;
	findBestMeshName("Mesh", meshName);

	G_MeshTemplate *t=new G_MeshTemplate(meshName,geoName,"default",PSHAPE_BOX,0);
	CMeshTemplateDlg dlg(t);

	if(dlg.DoModal()==IDCANCEL)
	{
		delete t;
		return;
	}

	G_EditorApp::getSingletonRef().getMap().getMeshes().registerData(t->getName(),t);
	addItem((DWORD_PTR)t, t->getName(), RI_MESH, _meshes, true);
}

void CResourceView::newParticleSystem()
{
	string partName;
	findBestParticleName("Particles", partName);
	G_ParticlesTemplate *gpt = new G_ParticlesTemplate(new ParticleSystemTemplate(0, 0), partName);
	G_EditorApp::getSingletonRef().getMap().getParticles().registerData(partName, gpt);
	addItem((DWORD_PTR)gpt, gpt->getName(), RI_PARTICLES, _particleSystems, true);
}

void CResourceView::newSound()
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE, _T("*.wav"), 0, OFN_EXPLORER, _T("Sounds (*.wav)|*.wav||"));

	if (file_dlg.DoModal() == IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());
	string shadowName;
	string soundName;

	if (!FileSystemFactory::getFileSystem()->extractLocalFileName(fullName, shadowName))
		return;

	findBestSoundName("Sound", soundName);
	G_SoundTemplate *t = new G_SoundTemplate(soundName);
	t->setSoundFile(shadowName);

	CSoundEditor dlg(t);
	if (dlg.DoModal() == IDCANCEL)
	{
		delete t;
		return;
	}

	G_EditorApp::getSingletonRef().getMap().getSounds().registerData(t->getName(), t);
	addItem((DWORD_PTR)t, t->getName(), RI_SOUND, _sounds, true);
}

void CResourceView::newGameMaterial()
{
	string matName;
	findBestParticleName("Material", matName);
	G_GameMaterial *gpt = new G_GameMaterial(matName,0);
	CGameMaterialEditor dlg(gpt);
	if (dlg.DoModal() == IDOK)
	{
		G_EditorApp::getSingletonRef().getMap().getGameMaterials().registerData(matName, gpt);
		addItem((DWORD_PTR)gpt, gpt->getName(), RI_GAMEMATERIAL, _gameMaterials, true);
	}
	else
		delete gpt;
}

void CResourceView::refreshResources()
{
	_view.DeleteAllItems();

	_materials=_view.InsertItem("Material",RI_MATERIAL,RI_MATERIAL);
	_meshes=_view.InsertItem("Meshes",RI_MESH, RI_MESH);
	_actors=_view.InsertItem("Actors",RI_ACTOR, RI_ACTOR);
	_particleSystems = _view.InsertItem("Particle systems", RI_PARTICLES, RI_PARTICLES);
	_sounds = _view.InsertItem("Sounds", RI_SOUND, RI_SOUND);
	_gameMaterials = _view.InsertItem("Game Materials", RI_GAMEMATERIAL, RI_GAMEMATERIAL);
	_entities = _view.InsertItem("Entities", RI_ENTITY, RI_ENTITY);

	const map<string,Material*>& mdb(MaterialSystem::getSingletonRef().getMaterials().getData());
	map<string,Material*>::const_iterator m(mdb.begin());
	for (; m != mdb.end(); ++m)
		addItem((DWORD_PTR)m->second, m->first, RI_MATERIAL, _materials, false);

	const map<string,G_MeshTemplate*>& tdb(G_EditorApp::getSingletonRef().getMap().getMeshes().getData());
	map<string,G_MeshTemplate*>::const_iterator t(tdb.begin());
	for (; t != tdb.end(); ++t)
		addItem((DWORD_PTR)t->second, t->first, RI_MESH, _meshes, false);

	const map<string, G_ParticlesTemplate*>& pdb(G_EditorApp::getSingletonRef().getMap().getParticles().getData());
	map<string, G_ParticlesTemplate*>::const_iterator p(pdb.begin());
	for (; p != pdb.end(); ++p)
		addItem((DWORD_PTR)p->second, p->first, RI_PARTICLES, _particleSystems, false);

	const map<string, G_SoundTemplate*>& sdb(G_EditorApp::getSingletonRef().getMap().getSounds().getData());
	map<string, G_SoundTemplate*>::const_iterator s(sdb.begin());
	for (; s != sdb.end(); ++s)
		addItem((DWORD_PTR)s->second, s->first, RI_SOUND, _sounds, false);

	const map<string, G_GameMaterial*>& gdb(G_EditorApp::getSingletonRef().getMap().getGameMaterials().getData());
	map<string, G_GameMaterial*>::const_iterator g(gdb.begin());
	for (; g != gdb.end(); ++g)
		addItem((DWORD_PTR)g->second, g->first, RI_GAMEMATERIAL, _gameMaterials, false);

	const map<string, Character*>& cdb(G_EditorApp::getSingletonRef().getCharacterDB().getData());
	map<string, Character*>::const_iterator c(cdb.begin());
	for (; c != cdb.end(); ++c)
		addItem((DWORD_PTR)c->second, c->first, RI_ACTOR, _actors, false);

	const map<string, G_EntityTemplate*>& edb(G_EditorApp::getSingletonRef().getEntityDB().getData());
	map<string, G_EntityTemplate*>::const_iterator e(edb.begin());
	for (; e != edb.end(); ++e)
		addItem((DWORD_PTR)e->second, e->first, RI_ENTITY, _entities, false);
}

void CResourceView::addItem(DWORD_PTR ptr, const string& name, int img, HTREEITEM parent, bool select)
{
	HTREEITEM h = _view.InsertItem(name.c_str(), img, img, parent);
	_view.SetItemData(h, ptr);

	if (select)
	{
		_view.Expand(parent, TVE_EXPAND);
		_view.Select(h, TVGN_CARET);
	}

}

void CResourceView::findBestMaterialName(const string& o, string& n) const
{
	if (MaterialSystem::getSingletonRef().getMaterial(o) == 0)
	{
		n = o;
		return;
	}

	stringstream matName;
	bool end = false;
	int i = 1;

	while (!end)
	{
		matName.str("");
		matName << o << i;

		end = MaterialSystem::getSingletonRef().getMaterial(matName.str()) == 0;

		++i;
	}

	n = matName.str();
}

void CResourceView::findBestMeshName(const string& o, string& n) const
{
	if (G_EditorApp::getSingletonRef().getMap().getMeshes().getData(o) == 0)
	{
		n = o;
		return;
	}

	stringstream name;
	bool end = false;
	int i = 1;

	while (!end)
	{
		name.str("");
		name << o << i;

		end = G_EditorApp::getSingletonRef().getMap().getMeshes().getData(name.str()) == 0;

		++i;
	}

	n = name.str();
}

void CResourceView::findBestParticleName(const string& o, string& n) const
{
	if (G_EditorApp::getSingletonRef().getMap().getParticles().getData(o) == 0)
	{
		n = o;
		return;
	}

	stringstream name;
	bool end = false;
	int i = 1;

	while (!end)
	{
		name.str("");
		name << o << i;

		end = G_EditorApp::getSingletonRef().getMap().getParticles().getData(name.str()) == 0;

		++i;
	}

	n = name.str();
}

void CResourceView::findBestSoundName(const string& o, string& n) const
{
	if (G_EditorApp::getSingletonRef().getMap().getSounds().getData(o) == 0)
	{
		n = o;
		return;
	}

	stringstream name;
	bool end = false;
	int i = 1;

	while (!end)
	{
		name.str("");
		name << o << i;

		end = G_EditorApp::getSingletonRef().getMap().getSounds().getData(name.str()) == 0;

		++i;
	}

	n = name.str();
}

void CResourceView::findBestGameMaterialName(const string& o, string& n) const
{
	if (G_EditorApp::getSingletonRef().getMap().getGameMaterials().getData(o) == 0)
	{
		n = o;
		return;
	}

	stringstream name;
	bool end = false;
	int i = 1;

	while (!end)
	{
		name.str("");
		name << o << i;

		end = G_EditorApp::getSingletonRef().getMap().getGameMaterials().getData(name.str()) == 0;

		++i;
	}

	n = name.str();
}
