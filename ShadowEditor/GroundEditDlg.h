#pragma once
#include "EditDlg.h"

#include "G_GroundToolPaint.h"
#include "G_GroundToolSetStartPos.h"
#include "G_GroundToolWakable.h"
#include "afxwin.h"

// CGroundEditDlg dialog

class CGroundEditDlg : public CEditDlg
{
	DECLARE_DYNAMIC(CGroundEditDlg)

public:
	CGroundEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGroundEditDlg();

// Dialog Data
	enum { IDD = IDD_GROUNDEDIT };

protected:

	G_GroundToolPaint					_tool;
	G_GroundToolSetStartPos				_startPosTool;
	G_GroundToolWakable					_wakable;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox _materials;
	afx_msg void OnBnClickedGroundPaint();
	afx_msg void OnBnClickedGroundErase();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeGroundMat();
	BOOL								_painting;
	BOOL								_erasing;
	afx_msg void OnBnClickedGroundRefreshmats();

	void								onToolChange();
	afx_msg void OnBnClickedGroundSpset();
	BOOL _settingStartPos;
	BOOL _wSet;
	BOOL _wPoint;
	BOOL _wPaint;
	afx_msg void OnBnClickedGroundWset();
	afx_msg void OnBnClickedGroundWpoint();
	afx_msg void OnBnClickedGroundWpaint();
protected:
	CComboBox _gameMaterials;
public:
	afx_msg void OnCbnSelchangeGroundGmat();
};
