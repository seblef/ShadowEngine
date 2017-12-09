#pragma once
#include "afxcmn.h"
#include "G_SoundTemplate.h"


// CSoundEditor dialog

class CSoundEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CSoundEditor)

public:
	CSoundEditor(G_SoundTemplate *t, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoundEditor();

// Dialog Data
	enum { IDD = IDD_SOUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	G_SoundTemplate*			_template;
	ISound*						_sound;
	bool						_fileChanged;

	CString _name;
	CSpinButtonCtrl _priority;
	CSpinButtonCtrl _radius;
	CSpinButtonCtrl _radiusAngle;
	BOOL _coneEnable;
	CSpinButtonCtrl _inAngle;
	CSpinButtonCtrl _outAngle;
	CSpinButtonCtrl _inVolume;
	CSpinButtonCtrl _outVolume;
	CSpinButtonCtrl _inLPF;
	CSpinButtonCtrl _outLPF;
	CSpinButtonCtrl _inReverb;
	CSpinButtonCtrl _outReverb;
public:
	afx_msg void OnBnClickedSndFile();
	afx_msg void OnBnClickedSndCone();
	afx_msg void OnBnClickedSndPlay();
	afx_msg void OnBnClickedOk();
};
