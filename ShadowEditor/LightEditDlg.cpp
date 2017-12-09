// LightEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "LightEditDlg.h"
#include "afxdialogex.h"
#include "G_EditorApp.h"
#include "G_Utils.h"


// CLightEditDlg dialog

IMPLEMENT_DYNAMIC(CLightEditDlg, CDialogEx)

CLightEditDlg::CLightEditDlg(Light* l, CWnd* pParent /*=NULL*/)
: CEditDlg(CLightEditDlg::IDD, pParent), _light(l), _init(false)
	, _intensity(l->getColor().a * 100.0f)
	, _range(l->getRange() * 10.0f)
	, _castShadows(l->getCastShadows() ? TRUE : FALSE)
	, _farAngle(0)
	, _nearAngle(0)
	, _nearDepth(0)
	, _width(0)
	, _height(0)
{

}

CLightEditDlg::~CLightEditDlg()
{
}

void CLightEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHT_COLOR, _color);
	DDX_Text(pDX, IDC_LIGHT_INT, _intensity);
	DDX_Control(pDX, IDC_LIGHT_INTSP, _intSP);
	DDX_Text(pDX, IDC_LIGHT_RANGE, _range);
	DDX_Control(pDX, IDC_LIGHT_RANGESP, _rangeSP);
	DDX_Check(pDX, IDC_LIGHT_CASTSHADOWS, _castShadows);
	DDX_Control(pDX, IDC_LIGHT_SMSIZE, _shadowMapSize);
	DDX_Control(pDX, IDC_LIGHT_SMFILTER, _shadowMapFilter);
	DDX_Text(pDX, IDC_LIGHT_FAR, _farAngle);
	DDX_Control(pDX, IDC_LIGHT_FARSP, _farSP);
	DDX_Text(pDX, IDC_LIGHT_NEAR, _nearAngle);
	DDX_Control(pDX, IDC_LIGHT_NEARSP, _nearSP);
	DDX_Text(pDX, IDC_LIGHT_DEPTH, _nearDepth);
	DDX_Text(pDX, IDC_LIGHT_WIDTH, _width);
	DDX_Control(pDX, IDC_LIGHT_WIDTHSP, _widthSP);
	DDX_Text(pDX, IDC_LIGHT_HEIGHT, _height);
	DDX_Control(pDX, IDC_LIGHT_HEIGHTSP, _heightSP);
	DDX_Control(pDX, IDC_LIGHT_DEPTHSP, _depthSP);
}


BEGIN_MESSAGE_MAP(CLightEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LIGHT_COLOR, &CLightEditDlg::OnBnClickedLightColor)
	ON_EN_CHANGE(IDC_LIGHT_INT, &CLightEditDlg::OnEnChangeLightInt)
	ON_EN_CHANGE(IDC_LIGHT_RANGE, &CLightEditDlg::OnEnChangeLightRange)
	ON_BN_CLICKED(IDC_LIGHT_CASTSHADOWS, &CLightEditDlg::OnBnClickedLightCastshadows)
	ON_CBN_SELCHANGE(IDC_LIGHT_SMSIZE, &CLightEditDlg::OnCbnSelchangeLightSmsize)
	ON_CBN_SELCHANGE(IDC_LIGHT_SMFILTER, &CLightEditDlg::OnCbnSelchangeLightSmfilter)
	ON_EN_CHANGE(IDC_LIGHT_FAR, &CLightEditDlg::OnEnChangeLightFar)
	ON_EN_CHANGE(IDC_LIGHT_NEAR, &CLightEditDlg::OnEnChangeLightNear)
	ON_EN_CHANGE(IDC_LIGHT_DEPTH, &CLightEditDlg::OnEnChangeLightDepth)
	ON_EN_CHANGE(IDC_LIGHT_WIDTH, &CLightEditDlg::OnEnChangeLightWidth)
	ON_EN_CHANGE(IDC_LIGHT_HEIGHT, &CLightEditDlg::OnEnChangeLightHeight)
END_MESSAGE_MAP()


// CLightEditDlg message handlers


BOOL CLightEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	enableShadows(_castShadows);
	_shadowMapSize.SetCurSel(getSMSizeSel(_light->getShadowMapSize()));

	int filter=0;
	int f=_light->getShadowMapFilter();
	if(f <= 1)			filter=0;
	else if(f <= 2)		filter=1;
	else if(f <= 4)		filter=2;
	else if(f <= 6)		filter=3;
	else if(f <= 8)		filter=4;
	else if(f <= 10)	filter=5;
	else if(f <= 12)	filter=6;
	else				filter=7;

	_shadowMapFilter.SetCurSel(filter);

	_color.SetColor(G_Utils::toColorRef(_light->getColor()));
	_intSP.SetRange32(0,10000);
	_rangeSP.SetRange32(1,1000);

	_intSP.SetPos32(_light->getColor().a * 100.0f);
	_rangeSP.SetPos32(_light->getRange() * 10.0f);

	_farSP.SetRange32(1,179);
	_nearSP.SetRange32(1,179);
	_depthSP.SetRange32(1,100);

	_widthSP.SetRange32(1,1000);
	_heightSP.SetRange32(1,1000);

	if(_light->getLightType()==Light::LT_SPOT)
	{
		enableSpot();
		LightSpot *s=(LightSpot*)_light;
		_farSP.SetPos32(s->getFarAngle());
		_nearSP.SetPos32(s->getNearAngle());
		_depthSP.SetPos32(s->getNearZ() * 10.0f);
	}
	else if(_light->getLightType()==Light::LT_AREA)
	{
		enableArea();
		enableSpot();

		LightArea* a=(LightArea*)_light;
		_farSP.SetPos32(a->getFarAngle());
		_nearSP.SetPos32(a->getNearAngle());
		_depthSP.SetPos32(a->getNearZ() * 10.0f);

		_widthSP.SetPos32(a->getAreaWidth() * 10.0f);
		_heightSP.SetPos32(a->getAreaHeight() * 10.0f);
	}

	_init=true;

	return TRUE;
}

void CLightEditDlg::enableShadows(BOOL enable)
{
	GetDlgItem(IDC_LIGHT_SMSIZE)->EnableWindow(enable);
	GetDlgItem(IDC_LIGHT_SMFILTER)->EnableWindow(enable);
}

void CLightEditDlg::enableArea()
{
	GetDlgItem(IDC_LIGHT_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_WIDTHSP)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_HEIGHTSP)->EnableWindow(TRUE);
}

void CLightEditDlg::enableSpot()
{
	GetDlgItem(IDC_LIGHT_FAR)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_FARSP)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_NEAR)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_NEARSP)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_DEPTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIGHT_DEPTHSP)->EnableWindow(TRUE);
}

void CLightEditDlg::OnBnClickedLightColor()
{
	float intensity=_light->getColor().a;
	Color c;
	G_Utils::toColor(_color.GetColor(),c);
	c.a=intensity;
	_light->setColor(c);
	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnEnChangeLightInt()
{
	if(!_init)			return;

	UpdateData(TRUE);
	Color c(_light->getColor());
	c.a=0.01f * (float)_intensity;
	_light->setColor(c);

	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnEnChangeLightRange()
{
	if(!_init)			return;

	UpdateData(TRUE);
	_light->setRange(0.1f * (float)_range);
	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnBnClickedLightCastshadows()
{
	UpdateData(TRUE);
	enableShadows(_castShadows);
	_light->setCastShadows(_castShadows ? true : false);
	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnCbnSelchangeLightSmsize()
{
	if(!_init)			return;

	const int sizes[]={64,128,256,512,1024,2048};
	int sel=_shadowMapSize.GetCurSel();
	if(sel!=-1)
	{
		_light->setShadowMapSize(sizes[sel]);
		G_EditorApp::getSingletonRef().refresh();
	}
}


void CLightEditDlg::OnCbnSelchangeLightSmfilter()
{
	if(!_init)			return;

	const int filters[]={1,2,4,6,8,10,12,16};
	int sel=_shadowMapFilter.GetCurSel();
	if(sel!=-1)
	{
		_light->setShadowMapFilter(filters[sel]);
		G_EditorApp::getSingletonRef().refresh();
	}
}


void CLightEditDlg::OnEnChangeLightFar()
{
	if(!_init)				return;

	UpdateData(TRUE);
	if(_light->getLightType()==Light::LT_SPOT)
		((LightSpot*)_light)->setFarAngle((float)_farAngle);
	else
		((LightArea*)_light)->setFarAngle((float)_farAngle);

	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnEnChangeLightNear()
{
	if(!_init)				return;

	UpdateData(TRUE);
	if(_light->getLightType()==Light::LT_SPOT)
		((LightSpot*)_light)->setNearAngle((float)_nearAngle);
	else
		((LightArea*)_light)->setNearAngle((float)_nearAngle);

	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnEnChangeLightDepth()
{
	if(!_init)				return;

	UpdateData(TRUE);
	if(_light->getLightType()==Light::LT_SPOT)
		((LightSpot*)_light)->setNearZ(0.1f * (float)_nearDepth);
	else
		((LightArea*)_light)->setNearZ(0.1f * (float)_nearDepth);

	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnEnChangeLightWidth()
{
	if(!_init)				return;

	UpdateData(TRUE);
	((LightArea*)_light)->setAreaWidth(0.1f * (float)_width);
	G_EditorApp::getSingletonRef().refresh();
}


void CLightEditDlg::OnEnChangeLightHeight()
{
	if(!_init)				return;

	UpdateData(TRUE);
	((LightArea*)_light)->setAreaHeight(0.1f * (float)_height);
	G_EditorApp::getSingletonRef().refresh();
}
