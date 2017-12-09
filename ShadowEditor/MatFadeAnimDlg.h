#pragma once
#include "afxcmn.h"
#include "MatAnimDlg.h"


// CMatFadeAnimDlg dialog

class CMatFadeAnimDlg : public CMatAnimDlg
{
	DECLARE_DYNAMIC(CMatFadeAnimDlg)

public:
	CMatFadeAnimDlg(Material *m, CMaterialPropertySheet *ps);
	virtual ~CMatFadeAnimDlg();

// Dialog Data
	enum { IDD = IDD_MAT_FADEANIM };

protected:

	void			enableCtrls(BOOL e);
	afx_msg void	updateVals();
	afx_msg void	OnBnClickedMatFadeenable();

	bool			_init;


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	CSpinButtonCtrl _start;
	CSpinButtonCtrl _end;
	BOOL _enable;
};
