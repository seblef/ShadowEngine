// SoundEditor.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "SoundEditor.h"
#include "afxdialogex.h"
#include "G_EditorApp.h"


// CSoundEditor dialog

IMPLEMENT_DYNAMIC(CSoundEditor, CDialogEx)

CSoundEditor::CSoundEditor(G_SoundTemplate *t, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoundEditor::IDD, pParent)
	, _template(t)
	, _sound(0)
	, _fileChanged(true)
	, _name(t->getName().c_str())
	, _coneEnable(t->getCone() ? TRUE : FALSE)
{

}

CSoundEditor::~CSoundEditor()
{
	if (_sound)
		delete _sound;
}

void CSoundEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SND_NAME, _name);
	DDX_Control(pDX, IDC_SND_PRIORSP, _priority);
	DDX_Control(pDX, IDC_SND_RADIUSSP, _radius);
	DDX_Control(pDX, IDC_SND_RANGLESP, _radiusAngle);
	DDX_Check(pDX, IDC_SND_CONE, _coneEnable);
	DDX_Control(pDX, IDC_SND_IANGLESP, _inAngle);
	DDX_Control(pDX, IDC_SND_OANGLESP, _outAngle);
	DDX_Control(pDX, IDC_SND_IVOLSP, _inVolume);
	DDX_Control(pDX, IDC_SND_OVOLSP, _outVolume);
	DDX_Control(pDX, IDC_SND_ILPFSP, _inLPF);
	DDX_Control(pDX, IDC_SND_OLPFSP, _outLPF);
	DDX_Control(pDX, IDC_SND_IREVSP, _inReverb);
	DDX_Control(pDX, IDC_SND_OREVSP, _outReverb);
}


BEGIN_MESSAGE_MAP(CSoundEditor, CDialogEx)
	ON_BN_CLICKED(IDC_SND_FILE, &CSoundEditor::OnBnClickedSndFile)
	ON_BN_CLICKED(IDC_SND_CONE, &CSoundEditor::OnBnClickedSndCone)
	ON_BN_CLICKED(IDC_SND_PLAY, &CSoundEditor::OnBnClickedSndPlay)
	ON_BN_CLICKED(IDOK, &CSoundEditor::OnBnClickedOk)
END_MESSAGE_MAP()


// CSoundEditor message handlers


BOOL CSoundEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_priority.SetRange32(0, 100);		_priority.SetPos32(_template->getPriority());
	_radius.SetRange32(1, 1000);		_radius.SetPos32((int)(_template->getRadius() * 10.0f));
	_radiusAngle.SetRange32(0, 360);	_radiusAngle.SetPos32((int)(_template->getRadiusAngle() * 180.0f / M_PI));

	_inAngle.SetRange32(0, 360);
	_outAngle.SetRange32(0, 360);
	_inVolume.SetRange32(0, 100);
	_outVolume.SetRange32(0, 100);
	_inLPF.SetRange32(0, 100);
	_outLPF.SetRange32(0, 100);
	_inReverb.SetRange32(0, 100);
	_outReverb.SetRange32(0, 100);

	G_SoundCone* c = _template->getCone();
	if (c)
	{
		_inAngle.SetPos32((int)(c->getInAngle() * 180.0f / M_PI));
		_outAngle.SetPos32((int)(c->getOutAngle() * 180.0f / M_PI));
		_inVolume.SetPos32((int)(c->getInVolume() * 100.0f));
		_outVolume.SetPos32((int)(c->getOutVolume() * 100.0f));
		_inLPF.SetPos32((int)(c->getInLPF() * 100.0f));
		_outLPF.SetPos32((int)(c->getOutLPF() * 100.0f));
		_inReverb.SetPos32((int)(c->getInReverb() * 100.0f));
		_outReverb.SetPos32((int)(c->getOutReverb() * 100.0f));
	}

	OnBnClickedSndCone();

	if (!_template->getSoundFile().empty())
		GetDlgItem(IDC_SND_FILE)->SetWindowText(_template->getSoundFile().c_str());

	return TRUE;
}


void CSoundEditor::OnBnClickedSndFile()
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE, _T("*.wav"), 0, OFN_EXPLORER, _T("Sounds (*.wav)|*.wav||"));

	if (file_dlg.DoModal() == IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());
	string soundName;

	FileSystemFactory::getFileSystem()->extractLocalFileName(fullName, soundName);

	GetDlgItem(IDC_SND_FILE)->SetWindowText(soundName.c_str());
	_fileChanged=true;
}


void CSoundEditor::OnBnClickedSndCone()
{
	const UINT res[] = {
		IDC_SND_IANGLE, IDC_SND_IANGLESP,
		IDC_SND_OANGLE, IDC_SND_OANGLESP,
		IDC_SND_IVOL, IDC_SND_IVOLSP,
		IDC_SND_OVOL, IDC_SND_OVOLSP,
		IDC_SND_ILPF, IDC_SND_ILPFSP,
		IDC_SND_OLPF, IDC_SND_OLPFSP,
		IDC_SND_IREV, IDC_SND_IREVSP,
		IDC_SND_OREV, IDC_SND_OREVSP
	};

	UpdateData(TRUE);

	for (int i = 0; i < 16; ++i)
		GetDlgItem(res[i])->EnableWindow(_coneEnable);
}


void CSoundEditor::OnBnClickedSndPlay()
{
	ISoundDevice* d = G_EditorApp::getSingletonRef().getAudio();
	d->getSource(0)->stop();

	if (_fileChanged)
	{
		_fileChanged = false;

		if (_sound)
		{
			delete _sound;
			_sound = 0;
		}

		CString file;
		GetDlgItem(IDC_SND_FILE)->GetWindowText(file);

		if (file != "<SoundFile>")
			_sound = d->loadSound((const char*)file);
	}

	if (_sound)
		d->getSource(0)->play(0, _sound);
}


void CSoundEditor::OnBnClickedOk()
{
	UpdateData(TRUE);
	_template->setName((const char*)_name);
	_template->setPriority(_priority.GetPos32());
	_template->setRadius(0.1f * (float)_radius.GetPos32());
	_template->setRadiusAngle(((float)_radiusAngle.GetPos32()) * M_PI / 180.0f);

	CString file;
	GetDlgItem(IDC_SND_FILE)->GetWindowText(file);
	if (file == "<SoundFile>")
		_template->setSoundFile("");
	else
		_template->setSoundFile((const char*)file);

	if (_coneEnable)
	{
		G_SoundCone* c = _template->getCone();
		if (!c)
		{
			c = new G_SoundCone;
			_template->setCone(c);
		}

		c->setInAngle(((float)_inAngle.GetPos32()) * M_PI / 180.0f);
		c->setOutAngle(((float)_outAngle.GetPos32()) * M_PI / 180.0f);
		c->setInVolume(0.01f * (float)_inVolume.GetPos32());
		c->setOutVolume(0.01f * (float)_outVolume.GetPos32());
		c->setInLPF(0.01f * (float)_inLPF.GetPos32());
		c->setOutLPF(0.01f * (float)_outLPF.GetPos32());
		c->setInReverb(0.01f * (float)_inReverb.GetPos32());
		c->setOutReverb(0.01f * (float)_outReverb.GetPos32());
	}
	else if (_template->getCone())
	{
		delete _template->getCone();
		_template->setCone(0);
	}

	CDialogEx::OnOK();
}
