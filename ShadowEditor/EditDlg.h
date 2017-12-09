#pragma once
#include "afxwin.h"

// CGroundEditDlg dialog

class CEditDlg : public CDialogEx
{
public:
	CEditDlg(UINT id, CWnd* pParent = NULL) : _id(id), CDialogEx(id, pParent)		{}   // standard constructor
	virtual ~CEditDlg()							{}

protected:

	UINT							_id;

public:

	UINT							getIDD() const				{ return _id; }
	virtual void					onToolChange()				{}
};
