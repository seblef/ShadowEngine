#pragma once

#include <StdRenderer.h>

class CMaterialPropertySheet;

// CMatTextureDlg dialog

class CMatTextureDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CMatTextureDlg)

public:
	CMatTextureDlg(Material *m, CMaterialPropertySheet *ps);
	virtual ~CMatTextureDlg();

// Dialog Data
	enum { IDD = IDD_MAT_TEXTURES };

protected:

	Material*					_mat;
	CMaterialPropertySheet*		_parent;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void						onTextureSelBtn(Material::TextureSlot ts);
	void						onTextureCheckBtn(Material::TextureSlot ts);

	afx_msg void				OnBnClickedMatDiffuset()						{ onTextureSelBtn(Material::TS_DIFFUSE); }
	afx_msg void				OnBnClickedBmatNormalt()						{ onTextureSelBtn(Material::TS_NORMAL); }
	afx_msg void				OnBnClickedMatSpect()							{ onTextureSelBtn(Material::TS_SPECULAR); }
	afx_msg void				OnBnClickedMatEmissivet()						{ onTextureSelBtn(Material::TS_EMISSIVE); }
	afx_msg void				OnBnClickedMatEnvtt()							{ onTextureSelBtn(Material::TS_ENVIONMENT); }

	afx_msg void				OnBnClickedMatDiffuseck()						{ onTextureCheckBtn(Material::TS_DIFFUSE); }
	afx_msg void				OnBnClickedMatNormalck()						{ onTextureCheckBtn(Material::TS_NORMAL); }
	afx_msg void				OnBnClickedMatSpecck()							{ onTextureCheckBtn(Material::TS_SPECULAR); }
	afx_msg void				OnBnClickedMatEmissiveck()						{ onTextureCheckBtn(Material::TS_EMISSIVE); }
	afx_msg void				OnBnClickedMatEnvtck()							{ onTextureCheckBtn(Material::TS_ENVIONMENT); }

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
