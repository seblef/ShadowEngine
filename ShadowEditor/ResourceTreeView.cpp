
#include "stdafx.h"
#include "resource.h"
#include "ResourceTreeView.h"
#include "MaterialPropertySheet.h"
#include "GameMaterialEditor.h"
#include "G_Creator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CResourceTree::CResourceTree()
{
}

CResourceTree::~CResourceTree()
{
}

BEGIN_MESSAGE_MAP(CResourceTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CResourceTree::OnNMDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

BOOL CResourceTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CResourceTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hSel=GetSelectedItem();
	if(hSel==0)
		return;

	int img,selimg;
	GetItemImage(hSel,img,selimg);
	DWORD_PTR d=GetItemData(hSel);

	if(!d)
		return;

	if(img==RI_MATERIAL)	//	Materials
	{
		CString name(GetItemText(hSel));
		Material *m=(Material*)d;
		CMaterialPropertySheet dlg(m,name,FALSE,"Material");
		dlg.DoModal();
	}
	else if(img==RI_MESH)		//		Meshes
	{
		G_MeshTemplate* t=(G_MeshTemplate*)d;
		G_Creator::createMesh(t);
	}
	else if (img == RI_ACTOR)	//		Actors
	{
		Character *c = (Character*)d;
		G_Creator::createActor(c);
	}
	else if (img == RI_PARTICLES)	//		Particles
	{
		G_ParticlesTemplate* t = (G_ParticlesTemplate*)d;
		G_Creator::createParticleSystem(t);
	}
	else if (img == RI_SOUND)	//		Sounds
	{
		G_SoundTemplate *t = (G_SoundTemplate*)d;
		G_Creator::createSound(t);
	}
	else if (img == RI_GAMEMATERIAL)
	{
		G_GameMaterial* mat = (G_GameMaterial*)d;
		string oldName = mat->getName();
		CGameMaterialEditor dlg(mat);
		if (dlg.DoModal() == IDOK)
		{
			if (mat->getName() != oldName)
				SetItemText(hSel, mat->getName().c_str());
		}
	}
	else if (img == RI_ENTITY)
	{
		G_EntityTemplate* et = (G_EntityTemplate*)d;
		G_Creator::createEntity(et);
	}

	*pResult=0;
}
