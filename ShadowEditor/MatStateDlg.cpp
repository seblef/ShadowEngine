// MatStateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MatStateDlg.h"
#include "afxdialogex.h"
#include "MaterialPropertySheet.h"
#include "G_EditorApp.h"


// CMatStateDlg dialog

IMPLEMENT_DYNAMIC(CMatStateDlg, CPropertyPage)

CMatStateDlg::CMatStateDlg(Material *m, CMaterialPropertySheet* ps)
	: CPropertyPage(CMatStateDlg::IDD), _mat(m), _parent(ps)
	, _alpha(m->getFlag(MF_ALPHA) ? TRUE : FALSE)
	, _castNoShadows(m->getFlag(MF_CASTNOSHADOWS) ? TRUE : FALSE)
{

}

CMatStateDlg::~CMatStateDlg()
{
}

void CMatStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_MAT_ALPHA, _alpha);
	DDX_Check(pDX, IDC_MAT_NOSHADOWS, _castNoShadows);
	DDX_Control(pDX, IDC_MAT_SRCBLEND, _srcBlend);
	DDX_Control(pDX, IDC_MAT_DESTBLEND, _destBlend);
	DDX_Control(pDX, IDC_MAT_CULL, _cull);
}


BEGIN_MESSAGE_MAP(CMatStateDlg, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_MAT_SRCBLEND, &CMatStateDlg::onBlendChanged)
	ON_CBN_SELCHANGE(IDC_MAT_DESTBLEND, &CMatStateDlg::onBlendChanged)
	ON_CBN_SELCHANGE(IDC_MAT_CULL, &CMatStateDlg::onCullChanged)
	ON_BN_CLICKED(IDC_MAT_ALPHA, &CMatStateDlg::onFlagChanged)
	ON_BN_CLICKED(IDC_MAT_NOSHADOWS, &CMatStateDlg::onFlagChanged)
END_MESSAGE_MAP()


// CMatStateDlg message handlers


BOOL CMatStateDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

//				Blend states
	for(int i=0;i<BLEND_COUNT;++i)
	{
		_srcBlend.AddString(g_BlendModeNames[i]);
		_destBlend.AddString(g_BlendModeNames[i]);
	}

	BlendMode src,dest;
	bool enable;
	G_EditorApp::getSingletonRef().getVideo()->getBlendStateDesc(_mat->getBlendState(),enable,src,dest);
	_srcBlend.SetCurSel(src);
	_destBlend.SetCurSel(dest);

	//				Cull mode
	for(int i=0;i<CULL_COUNT;++i)
		_cull.AddString(g_CullModeNames[i]);

	CullMode cm;
	FillMode fm;
	G_EditorApp::getSingletonRef().getVideo()->getRenderStateDesc(_mat->getRenderState(),cm,fm);
	_cull.SetCurSel(cm);

	return TRUE;
}


void CMatStateDlg::onBlendChanged()
{
	BlendMode src,dest;
	src=(BlendMode)_srcBlend.GetCurSel();
	dest=(BlendMode)_destBlend.GetCurSel();

	G_EditorApp::getSingletonRef().getVideo()->destroyBlendState(_mat->getBlendState());
	_mat->setBlendState(G_EditorApp::getSingletonRef().getVideo()->createBlendState(src!=BLEND_ONE || dest!=BLEND_ZERO,src,dest));

	_parent->refresh();
}

void CMatStateDlg::onCullChanged()
{
	CullMode cm=(CullMode)_cull.GetCurSel();

	G_EditorApp::getSingletonRef().getVideo()->destroyRenderState(_mat->getRenderState());
	_mat->setRenderState(G_EditorApp::getSingletonRef().getVideo()->createRenderState(cm));

	_parent->refresh();
}

void CMatStateDlg::onFlagChanged()
{
	UpdateData(TRUE);

	unsigned int f=MF_ALPHA | MF_CASTNOSHADOWS;
	_mat->unsetFlag(f);

	f=0;
	if(_alpha)			f|=MF_ALPHA;
	if(_castNoShadows)	f|=MF_CASTNOSHADOWS;

	_mat->setFlag(f);
	_parent->refresh();
}
