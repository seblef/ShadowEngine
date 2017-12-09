#pragma once

#include "MatColorDlg.h"
#include "MatTextureDlg.h"
#include "MatStateDlg.h"
#include "MatUVDlg.h"
#include "MatUVAnimDlg.h"
#include "MatTextureAnimDlg.h"
#include "MatFadeAnimDlg.h"
#include "CPreviewMesh.h"
#include "G_MaterialSave.h"

// CMaterialPropertySheet

class CMaterialPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMaterialPropertySheet)

public:
	CMaterialPropertySheet(Material* m, const CString& matName,  BOOL editName, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMaterialPropertySheet(Material* m, const CString& matName,  BOOL editName, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMaterialPropertySheet();

protected:

	CPreviewMesh			_preview;
	Material*				_mat;
	BOOL					_editName;

	G_MaterialSave			_save;

	CMatColorDlg			_colorDlg;
	CMatTextureDlg			_textureDlg;
	CMatStateDlg			_stateDlg;
	CMatUVDlg				_uvDlg;
	CMatUVAnimDlg			_uvAnimDlg;
	CMatTextureAnimDlg		_texAnimDlg;
	CMatFadeAnimDlg			_fadeAnimDlg;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL			OnInitDialog();
	virtual BOOL			OnCommand(WPARAM wParam, LPARAM lParam);

	void					refresh()							{ _preview.update(0); }
	const CString&			getName() const						{ return _colorDlg.getName(); }
};


