// ParticlesEditor.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "ParticlesEditor.h"
#include "ParticleMaterial.h"
#include "afxdialogex.h"
#include "SelectionDlg.h"
#include "G_ParticlesTemplateSaver.h"

// CParticlesEditor dialog

IMPLEMENT_DYNAMIC(CParticlesEditor, CDialogEx)

CParticlesEditor::CParticlesEditor(ParticleSystemTemplate *t, const string& name, CWnd* pParent /*=NULL*/)
	: CDialogEx(CParticlesEditor::IDD, pParent)
	, _name(name)
	, _showEmitter(TRUE)
	, _preview(G_EditorApp::getSingletonRef().getVideo())
	, _template(t)
	, _emitterProps(this,&_changed,&_name)
	, _system(*t)
	, _changed(false)
	, _newTemplate(0)
{
	_preview.setParticleSystem(_system.getSystem());
}

CParticlesEditor::~CParticlesEditor()
{
}

void CParticlesEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PS_PREVIEW, _preview);
	DDX_Control(pDX, IDC_PS_EMITTERS, _emitterList);
	DDX_Check(pDX, IDC_PS_SHOWEMITTER, _showEmitter);
}


BEGIN_MESSAGE_MAP(CParticlesEditor, CDialogEx)
	ON_CBN_SELCHANGE(IDC_PS_EMITTERS, &CParticlesEditor::OnCbnSelchangePsEmitters)
	ON_BN_CLICKED(IDC_PS_SHOWEMITTER, &CParticlesEditor::OnBnClickedPsShowemitter)
	ON_BN_CLICKED(IDC_PS_ADDEMITTER, &CParticlesEditor::OnBnClickedPsAddemitter)
	ON_BN_CLICKED(IDC_PS_REMEMITTER, &CParticlesEditor::OnBnClickedPsRememitter)
	ON_BN_CLICKED(IDC_PS_EXPORT, &CParticlesEditor::OnBnClickedPsExport)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_PS_MATERIAL, &CParticlesEditor::OnBnClickedPsMaterial)
	ON_BN_CLICKED(IDOK, &CParticlesEditor::OnBnClickedOk)
END_MESSAGE_MAP()


// CParticlesEditor message handlers


BOOL CParticlesEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_preview.initDialog();
	this->SetTimer(1, 40,0);

	refreshEmitterList();

	return TRUE;
}


void CParticlesEditor::OnCbnSelchangePsEmitters()
{
	int sel = _emitterList.GetCurSel();
	if (sel == -1)
		_emitterProps.setTemplate(0);
	else
		_emitterProps.setTemplate(_system.getEmitter(sel));

	_emitterProps.AdjustLayout();
}


void CParticlesEditor::OnBnClickedPsShowemitter()
{
	// TODO: Add your control notification handler code here
}


void CParticlesEditor::OnBnClickedPsAddemitter()
{
	KillTimer(1);

	vector<string> emitters;
	emitters.push_back("point");
	emitters.push_back("linear");

	CSelectionDlg dlg(&emitters);
	if (dlg.DoModal() == IDOK)
	{
		_system.addEmitter(ParticleEmitterFactory::createParticleEmitter(dlg.getSelectedItem(),
			ParticleEngine::getSingletonRef().getPointRenderer(),
			new ParticleMaterial("",false),
			1000,
			Vector3::NullVector,
			ParticleEmitterParams(),
			ParticleEmitterParams(),
			50.0f));

		_preview.setParticleSystem(_system.getSystem());

		int sel=_emitterList.AddString(dlg.getSelectedItem().c_str());
		_emitterList.SetCurSel(sel);
		OnCbnSelchangePsEmitters();

		_changed = true;
	}

	this->SetTimer(1, 40, 0);
}


void CParticlesEditor::OnBnClickedPsRememitter()
{
	int sel = _emitterList.GetCurSel();
	if (sel != -1)
	{
		KillTimer(1);

		_system.remEmitter(sel);
		_preview.setParticleSystem(_system.getSystem());
		_emitterList.DeleteString(sel);

		if (_emitterList.GetCount() > 0)
		{
			if (sel == _emitterList.GetCount())
				--sel;

			_emitterList.SetCurSel(sel);
			OnCbnSelchangePsEmitters();
		}

		_changed = true;

		this->SetTimer(1, 25, 0);
	}
}


void CParticlesEditor::OnBnClickedPsExport()
{
	ParticleSystemTemplate *t=_system.buildTemplate();

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	FileSystemFactory::getFileSystem()->changeWorkingDirectory("Particles/");

	CFileDialog file_dlg(FALSE, _T("*.txt"), 0, OFN_EXPLORER, _T("Particle system (*.txt)|*.txt||"));

	if (file_dlg.DoModal() == IDOK)
	{
		ofstream ofs((const char*)file_dlg.GetPathName(), fstream::out);
		if (ofs.good())
		{
			G_ParticlesTemplateSaver saver;
			saver.saveTemplate(*t, ofs, _name);
		}
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	delete t;
}

void CParticlesEditor::OnTimer(UINT_PTR nIDEvent)
{
	_preview.update(0.04f);

	CDialogEx::OnTimer(nIDEvent);
}

void CParticlesEditor::refreshEmitterList()
{
	_emitterList.ResetContent();

	for (int i = 0; i < _system.getEmitterCount(); ++i)
		_emitterList.AddString(_system.getEmitter(i)->getEmitterName());

	_emitterList.SetCurSel(0);
	OnCbnSelchangePsEmitters();

	_showEmitter = TRUE;
}

int CParticlesEditor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	rect.SetRect(321, 7, 321 + 211, 7 + 270);

	_emitterProps.Create(WS_VISIBLE | WS_CHILD | WS_BORDER, rect, this, 2);
	_emitterProps.EnableHeaderCtrl();
	_emitterProps.EnableDescriptionArea();
	_emitterProps.SetVSDotNetLook(TRUE);
	return 0;
}


void CParticlesEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED)
	{
		CRect rect;
		GetClientRect(&rect);

		rect.left = rect.right - 330;
		rect.bottom = rect.top + 450;

		_emitterProps.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


void CParticlesEditor::OnBnClickedPsMaterial()
{
	int sel = _emitterList.GetCurSel();
	if (sel != -1)
	{
		CParticleMaterial dlg(_system.getEmitter(sel)->getMaterial());
		if (dlg.DoModal() == IDOK && dlg.materialHasChanged())
		{
			ParticleMaterial *new_mat = dlg.getNewMaterial();
			_system.getEmitter(sel)->setMaterial(new_mat);
			_changed = true;
		}
	}
}


void CParticlesEditor::OnBnClickedOk()
{
	if (_changed)
		_newTemplate = _system.buildTemplate();

	CDialogEx::OnOK();
}
