#pragma once
#include "afxwin.h"
#include "afxpropertygridctrl.h"
#include "CPreviewParticles.h"
#include "CParticlesPropertyGrid.h"
#include "G_EditorParticleSystem.h"


// CParticlesEditor dialog

class CParticlesEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CParticlesEditor)

public:
	CParticlesEditor(ParticleSystemTemplate *t, const string& name, CWnd* pParent = NULL);   // standard constructor
	virtual ~CParticlesEditor();

// Dialog Data
	enum { IDD = IDD_PARTICLEEDIT };

protected:

	void						refreshEmitterList();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CPreviewParticles			_preview;
	CParticlesPropertyGrid		_emitterProps;
	CComboBox					_emitterList;
	BOOL						_showEmitter;

	ParticleSystemTemplate*		_template;
	string						_name;
	bool						_changed;
	ParticleSystemTemplate*		_newTemplate;
	G_EditorParticleSystem		_system;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangePsEmitters();
	afx_msg void OnBnClickedPsShowemitter();
	afx_msg void OnBnClickedPsAddemitter();
	afx_msg void OnBnClickedPsRememitter();
	afx_msg void OnBnClickedPsExport();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedPsMaterial();
	afx_msg void OnBnClickedOk();

	ParticleSystemTemplate*		getNewTemplate() const				{ return _newTemplate; }
	const string&				getName() const						{ return _name;  }
};
