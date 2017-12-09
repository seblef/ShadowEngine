#pragma once
#include "MatAnimDlg.h"


// CMatUVAnimDlg dialog

class CMatUVAnimDlg : public CMatAnimDlg
{
	DECLARE_DYNAMIC(CMatUVAnimDlg)

public:
	CMatUVAnimDlg(Material *m, CMaterialPropertySheet *ps);
	virtual ~CMatUVAnimDlg();

// Dialog Data
	enum { IDD = IDD_MAT_UVANIM };

protected:
	BOOL				_enable;
	CSpinButtonCtrl		_uOffSP;
	CSpinButtonCtrl		_vOffSP;
	CSpinButtonCtrl		_rotSP;

	bool				_init;

	void				enableCtrls(BOOL e);
	afx_msg void		updateVals();
	afx_msg void		OnBnClickedMuvEnable();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
