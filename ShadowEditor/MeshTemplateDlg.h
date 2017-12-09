#pragma once
#include "afxwin.h"
#include "G_MeshTemplateSave.h"
#include "CPreviewMesh.h"

// CMeshTemplateDlg dialog

class CMeshTemplateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshTemplateDlg)

public:
	CMeshTemplateDlg(G_MeshTemplate *t, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeshTemplateDlg();

// Dialog Data
	enum { IDD = IDD_MESHTEMPLATE };

protected:

	G_MeshTemplateSave				_save;
	G_MeshTemplate*					_mesh;

	void							refreshMaterialList();
	void							refreshGameMaterialList();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	CPreviewMesh					_preview;
	CString							_name;
	CString							_geoName;
	CComboBox						_materials;
	CComboBox						_shapes;
	BOOL							_noCollision;
public:
	afx_msg void OnCbnSelchangeMtMat();
	afx_msg void OnBnClickedMtMatedit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	const CString&					getName() const					{ return _name; }
	afx_msg void OnBnClickedMtGmatedit();
protected:
	CComboBox _gameMaterials;
};
