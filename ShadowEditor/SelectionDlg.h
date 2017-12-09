#pragma once
#include "afxwin.h"


// CSelectionDlg dialog

class CSelectionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectionDlg)

protected:

	vector<string>				_items;
	string						_selected;

public:
	CSelectionDlg(const vector<string>* items, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectionDlg();

// Dialog Data
	enum { IDD = IDD_LISTSEL };

	void						setItems(const vector<string>& items)				{ _items=items; }
	void						addItem(const string& item)							{ _items.push_back(item); }

	const string&				getSelectedItem() const								{ return _selected; }

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CListBox _list;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
