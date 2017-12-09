
// ShadowFontDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "../Renderer2D/R2D_FontCharArea.h"


// CShadowFontDlg dialog
class CShadowFontDlg : public CDialogEx
{
// Construction
public:
	CShadowFontDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SHADOWFONT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	void				preview();
	void				fillFontList();
	void				saveTable(const R2D_FontCharArea* table) const;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	CString _name;
	int _texSize;
	CListBox _fontNames;
	CListBox _fontSize;
	CStatic _preview;
	afx_msg void OnLbnSelchangeFontname();
	afx_msg void OnLbnSelchangeFontsize();
	afx_msg void OnBnClickedApply();
	BOOL _bold;
	BOOL _italic;
};
