#pragma once
#include "afxwin.h"

#include <StdRenderer.h>

class CMaterialPropertySheet;

// CMatStateDlg dialog

class CMatStateDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CMatStateDlg)

public:
	CMatStateDlg(Material *m, CMaterialPropertySheet* ps);
	virtual ~CMatStateDlg();

// Dialog Data
	enum { IDD = IDD_MAT_STATE };

protected:

	Material*				_mat;
	CMaterialPropertySheet*	_parent;

	afx_msg void				onBlendChanged();
	afx_msg void				onFlagChanged();
	afx_msg void				onCullChanged();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL				OnInitDialog();
protected:
	BOOL						_alpha;
	BOOL						_castNoShadows;
	CComboBox					_srcBlend;
	CComboBox					_destBlend;
	CComboBox					_cull;
};
