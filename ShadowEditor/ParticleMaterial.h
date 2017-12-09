#pragma once
#include "afxwin.h"
#include <ParticleLib.h>


// CParticleMaterial dialog

class CParticleMaterial : public CDialogEx
{
	DECLARE_DYNAMIC(CParticleMaterial)

public:
	CParticleMaterial(ParticleMaterial *m, CWnd* pParent = NULL);   // standard constructor
	virtual ~CParticleMaterial();

// Dialog Data
	enum { IDD = IDD_PARTICLEMATERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL		OnInitDialog();
protected:
	int					_textureType;
	CListBox			_setList;
	BOOL				_blendEnable;
	CComboBox			_srcBlend;
	CComboBox			_destBlend;

	ParticleMaterial*	_material;
	bool				_changed;

	void				enableSetCtrls(BOOL enable);
	bool				selectTexture(string& texName);
	ParticleMaterial*	createNewMaterial();

public:
	afx_msg void		OnBnClickedPsmSingle();
	afx_msg void		OnBnClickedPsmTex();
	afx_msg void		OnBnClickedPsmAdd();
	afx_msg void		OnBnClickedPsmRem();
	afx_msg void		OnBnClickedPsmUp();
	afx_msg void		OnBnClickedPsmDown();
	afx_msg void		OnMaterialChanged()				{ _changed = true;  }
	afx_msg void		OnBnClickedOk();

	bool				materialHasChanged() const		{ return _changed; }
	ParticleMaterial*	getNewMaterial() const			{ return _material;  }
};
