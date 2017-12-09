#pragma once
#include "afxcolorbutton.h"
#include "afxcmn.h"

#include <StdRenderer.h>

class CMaterialPropertySheet;

// CMatColorDlg dialog

class CMatColorDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CMatColorDlg)

public:
	CMatColorDlg(Material *m, CMaterialPropertySheet* ps, const CString& matName, BOOL editName);
	virtual ~CMatColorDlg();

// Dialog Data
	enum { IDD = IDD_MAT_COLOR };

protected:

	afx_msg void				updateValues();
	afx_msg void				OnEnChangeMatName();

	Material*					_mat;
	CMaterialPropertySheet*		_parent;
	BOOL						_editName;

	bool						_init;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL				OnInitDialog();
	const CString&				getName() const					{ return _name; }

protected:
	CString						_name;
	CMFCColorButton				_ambientBtn;
	CMFCColorButton				_diffuseBtn;
	CMFCColorButton				_specBtn;
	CSpinButtonCtrl				_shininessSP;
	CSpinButtonCtrl				_specIntensity;
};
