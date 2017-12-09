
#pragma once

#include "ResourceTreeView.h"
#include "ResourceToolBar.h"
#include <StdRenderer.h>
#include "G_MeshTemplate.h"
#include "G_ParticlesTemplate.h"
#include "G_SoundTemplate.h"
#include "G_GameMaterial.h"

class CResourceView : public CDockablePane
{
// Construction
public:
	CResourceView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributs
protected:

	void						addItem(DWORD_PTR ptr, const string& name, int img, HTREEITEM parent, bool select);

	void						newMaterial();
	void						newMesh();
	void						newParticleSystem();
	void						newSound();
	void						newGameMaterial();

	void						openMaterial();
	void						openParticleSystem();

	void						findBestMaterialName(const string& original, string& newName) const;
	void						findBestMeshName(const string& original, string& newName) const;
	void						findBestParticleName(const string& original, string& newName) const;
	void						findBestSoundName(const string& original, string& newName) const;
	void						findBestGameMaterialName(const string& original, string& newName) const;

	CResourceToolBar			_toolBar;
	CResourceTree				_view;
	CImageList					_images;

	HTREEITEM					_materials;
	HTREEITEM					_meshes;
	HTREEITEM					_actors;
	HTREEITEM					_particleSystems;
	HTREEITEM					_sounds;
	HTREEITEM					_gameMaterials;
	HTREEITEM					_entities;

// Implémentation
public:
	virtual ~CResourceView();

	void						refreshResources();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnResNew();
	afx_msg void OnResOpen();
	afx_msg void OnResEdit();
};

