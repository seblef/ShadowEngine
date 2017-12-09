#pragma once
#include "afxcmn.h"
#include "afxcolorbutton.h"
#include "G_Map.h"

// CMapPropertiesDlg dialog

class CMapPropertiesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapPropertiesDlg)

public:
	CMapPropertiesDlg(G_Map* m, bool newMap, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapPropertiesDlg();

// Dialog Data
	enum { IDD = IDD_MAP_PROPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	bool				_newMap;
	G_Map*				_map;

	CString				_name;
	UINT				_width;
	UINT				_height;
	BOOL				_sunEnable;
	UINT				_sunInt;
	int					_XRot;
	int					_YRot;
	int					_ZRot;
	CSpinButtonCtrl		_widthSP;
	CSpinButtonCtrl		_heightSP;
	CMFCColorButton		_ambientBtn;
	CMFCColorButton		_sunBtn;
	CSpinButtonCtrl		_intSp;
	CSpinButtonCtrl		_XRotSP;
	CSpinButtonCtrl		_YRotSP;
	CSpinButtonCtrl		_ZRotSP;

	Color				_ambient;
	Color				_sunColor;
	Vector3				_direction;


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	const char*			getName() const				{ return _name; }
	
	int					getWidth() const			{ return _width; }
	int					getHeight() const			{ return _height; }

	bool				isSunEnable() const			{ return _sunEnable!=0; }
	float				getSunIntensity() const		{ return ((float)_sunInt) * 0.01f; }
	const Vector3&		getSunDirection() const		{ return _direction; }
	const Color&		getSunColor() const			{ return _sunColor; }
	const Color&		getAmbient() const			{ return _ambient; }


	afx_msg void OnBnClickedMapSunenable();
};
