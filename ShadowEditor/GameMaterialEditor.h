#pragma once

#include "G_GameMaterial.h"
#include "afxwin.h"

// CGameMaterialEditor dialog

class CGameMaterialEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CGameMaterialEditor)

public:
	CGameMaterialEditor(G_GameMaterial *mat, CWnd* pParent = NULL);   // standard constructor
	virtual ~CGameMaterialEditor();

// Dialog Data
	enum { IDD = IDD_GMATERIAL };

protected:

	G_GameMaterial*			_material;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CString _name;
	CComboBox _debris;
public:
	afx_msg void OnBnClickedOk();
};
