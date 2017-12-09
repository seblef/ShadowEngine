#pragma once
#include "afxcmn.h"

#include <StdRenderer.h>

class CMaterialPropertySheet;

// CMatUVDlg dialog

class CMatUVDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CMatUVDlg)

public:
	CMatUVDlg(Material *m, CMaterialPropertySheet *ps);
	virtual ~CMatUVDlg();

// Dialog Data
	enum { IDD = IDD_MAT_UV };

protected:

	Material*				_mat;
	CMaterialPropertySheet*	_parent;
	bool					_init;

	CSpinButtonCtrl			_uOffset;
	CSpinButtonCtrl			_uScale;
	CSpinButtonCtrl			_vOffset;
	CSpinButtonCtrl			_vScale;
	CSpinButtonCtrl			_rot;

	afx_msg void			updateValues();

	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL			OnInitDialog();
};
