#pragma once
#include "afxcolorbutton.h"
#include "afxcmn.h"
#include "afxwin.h"

#include "EditDlg.h"

#include <StdRenderer.h>

// CLightEditDlg dialog

class CLightEditDlg : public CEditDlg
{
	DECLARE_DYNAMIC(CLightEditDlg)

public:
	CLightEditDlg(Light* l, CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightEditDlg();

// Dialog Data
	enum { IDD = IDD_LIGHTEDIT };

protected:

	bool			_init;
	Light*			_light;

	void			enableShadows(BOOL enable);
	void			enableSpot();
	void			enableArea();

	int				getSMSizeSel(int smSize) const
	{
		if(smSize <= 64)			return 0;
		else if(smSize <= 128)		return 1;
		else if(smSize <= 256)		return 2;
		else if(smSize <= 512)		return 3;
		else if(smSize <= 1024)		return 4;
		else if(smSize <= 2048)		return 5;
		else						return 0;
	}

	int				getSMFilterSel(int f) const
	{
		int r=0;
		switch(f)
		{
		case 1:			r=0;		break;
		case 2:			r=1;		break;
		case 4:			r=2;		break;
		case 6:			r=3;		break;
		case 8:			r=4;		break;
		case 10:		r=5;		break;
		case 12:		r=6;		break;
		case 16:		r=7;		break;
		};

		return r;
	}

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CMFCColorButton				_color;
	UINT						_intensity;
	CSpinButtonCtrl				_intSP;
	UINT						_range;
	CSpinButtonCtrl				_rangeSP;
	BOOL						_castShadows;
	CComboBox					_shadowMapSize;
	CComboBox					_shadowMapFilter;
	int							_farAngle;
	CSpinButtonCtrl				_farSP;
	UINT						_nearAngle;
	CSpinButtonCtrl				_nearSP;
	int							_nearDepth;
	UINT						_width;
	CSpinButtonCtrl				_widthSP;
	UINT						_height;
	CSpinButtonCtrl				_heightSP;
public:
	afx_msg void OnBnClickedLightColor();
	afx_msg void OnEnChangeLightInt();
	afx_msg void OnEnChangeLightRange();
	afx_msg void OnBnClickedLightCastshadows();
	afx_msg void OnCbnSelchangeLightSmsize();
	afx_msg void OnCbnSelchangeLightSmfilter();
	afx_msg void OnEnChangeLightFar();
	afx_msg void OnEnChangeLightNear();
	afx_msg void OnEnChangeLightDepth();
	afx_msg void OnEnChangeLightWidth();
	afx_msg void OnEnChangeLightHeight();
	CSpinButtonCtrl _depthSP;
};
