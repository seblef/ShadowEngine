#pragma once
#include "MatAnimDlg.h"

// CMatTextureAnimDlg dialog

class CMatTextureAnimDlg : public CMatAnimDlg
{
	DECLARE_DYNAMIC(CMatTextureAnimDlg)

public:
	CMatTextureAnimDlg(Material* m, CMaterialPropertySheet *ps);
	virtual ~CMatTextureAnimDlg();

// Dialog Data
	enum { IDD = IDD_MAT_TEXANIM };

protected:
	void			enableCtrls(BOOL e);
	afx_msg void	updateVals();
	afx_msg void	OnBnClickedMtexEnable();

	bool			_init;

	BOOL			_enable;
	UINT			_time;
	CSpinButtonCtrl _rowSP;
	CSpinButtonCtrl _colSP;
	CSpinButtonCtrl _countSP;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
