#pragma once

#include "EditDlg.h"

// CNullEditDlg dialog

class CNullEditDlg : public CEditDlg
{
	DECLARE_DYNAMIC(CNullEditDlg)

public:
	CNullEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNullEditDlg();

// Dialog Data
	enum { IDD = IDD_NULLEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
