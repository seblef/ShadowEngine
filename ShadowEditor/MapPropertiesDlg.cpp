// MapPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MapPropertiesDlg.h"
#include "afxdialogex.h"
#include "G_Utils.h"
#include <StdRenderer.h>


// CMapPropertiesDlg dialog

IMPLEMENT_DYNAMIC(CMapPropertiesDlg, CDialogEx)

CMapPropertiesDlg::CMapPropertiesDlg(G_Map *m, bool newMap, CWnd* pParent /*=NULL*/)
: CDialogEx(CMapPropertiesDlg::IDD, pParent), _newMap(newMap), _map(m)
	, _name(newMap ? "new_map" : m->getName().c_str())
	, _width()
	, _height()
	, _sunEnable(TRUE)
	, _sunInt(0)
	, _XRot(0)
	, _YRot(0)
	, _ZRot(0)
{

}

CMapPropertiesDlg::~CMapPropertiesDlg()
{
}

void CMapPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAP_NAME, _name);
	DDX_Text(pDX, IDC_MAP_WIDTH, _width);
	DDX_Text(pDX, IDC_MAP_HEIGHT, _height);
	DDX_Check(pDX, IDC_MAP_SUNENABLE, _sunEnable);
	DDX_Text(pDX, IDC_MAP_SUNINT, _sunInt);
	DDX_Text(pDX, IDC_MAP_XAXIS, _XRot);
	DDX_Text(pDX, IDC_MAP_YAXIS, _YRot);
	DDX_Control(pDX, IDC_MAP_WIDTHSP, _widthSP);
	DDX_Control(pDX, IDC_MAP_HEIGHTSP, _heightSP);
	DDX_Control(pDX, IDC_MAP_AMBIENT, _ambientBtn);
	DDX_Control(pDX, IDC_MAP_SUNCOLOR, _sunBtn);
	DDX_Control(pDX, IDC_MAP_SUNINTSP, _intSp);
	DDX_Control(pDX, IDC_MAP_XAXISSP, _XRotSP);
	DDX_Control(pDX, IDC_MAP_YAXISSP, _YRotSP);
	DDX_Text(pDX, IDC_MAP_ZAXIS, _ZRot);
	DDX_Control(pDX, IDC_MAP_ZAXISSP, _ZRotSP);
}


BEGIN_MESSAGE_MAP(CMapPropertiesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMapPropertiesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MAP_SUNENABLE, &CMapPropertiesDlg::OnBnClickedMapSunenable)
END_MESSAGE_MAP()


// CMapPropertiesDlg message handlers


BOOL CMapPropertiesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_widthSP.SetRange32(10,10000);
	_heightSP.SetRange32(10,10000);
	_intSp.SetRange32(0,10000);
	_XRotSP.SetRange32(-100,100);
	_YRotSP.SetRange32(-100,100);
	_ZRotSP.SetRange32(-100,100);

	if(_newMap)
	{
		_widthSP.SetPos32(50);
		_heightSP.SetPos32(50);
		_intSp.SetPos32(100);
		_ambientBtn.SetColor(RGB(25,25,25));
		_sunBtn.SetColor(RGB(200,175,175));
		_XRotSP.SetPos32(0);
		_YRotSP.SetPos32(-100);
		_ZRotSP.SetPos32(0);
	}
	else
	{
		const LightGlobal& b(LightSystem::getSingletonRef().getGlobalLight());

		BOOL enable=b.isGlobalLightEnable() ? TRUE : FALSE;

		_sunEnable=enable;
		((CButton*)GetDlgItem(IDC_MAP_SUNENABLE))->SetCheck(enable ? BST_CHECKED : BST_UNCHECKED);
		_widthSP.SetPos32(_map->getGround().getWidth());
		_heightSP.SetPos32(_map->getGround().getHeight());
		_intSp.SetPos32(b.getColor().a * 100.0f);
		_sunBtn.SetColor(G_Utils::toColorRef(b.getColor()));
		_ambientBtn.SetColor(G_Utils::toColorRef(b.getAmbientLight()));

		_XRotSP.SetPos32(b.getDirection().x * 100.0f);
		_YRotSP.SetPos32(b.getDirection().y * 100.0f);
		_ZRotSP.SetPos32(b.getDirection().z * 100.0f);
	}

	return TRUE;
}


void CMapPropertiesDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	G_Utils::toColor(_ambientBtn.GetColor(),_ambient);
	G_Utils::toColor(_sunBtn.GetColor(),_sunColor);
	_sunColor.a=((float)_intSp.GetPos32()) * 0.01f;

	_direction.x=0.01f * (float)_XRot;
	_direction.y=0.01f * (float)_YRot;
	_direction.z=0.01f * (float)_ZRot;
	_direction.normalize();

	CDialogEx::OnOK();
}


void CMapPropertiesDlg::OnBnClickedMapSunenable()
{
	UpdateData(TRUE);

	UINT items[]={
		IDC_MAP_SUNCOLOR,
		IDC_MAP_SUNINT,
		IDC_MAP_SUNINTSP,
		IDC_MAP_XAXIS,
		IDC_MAP_XAXISSP,
		IDC_MAP_YAXIS,
		IDC_MAP_YAXISSP
	};

	for(int i=0;i<7;++i)
		GetDlgItem(items[i])->EnableWindow(_sunEnable);
}
