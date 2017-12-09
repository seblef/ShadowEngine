#pragma once
#include "afxwin.h"

#include <StdRenderer.h>

class CMaterialPropertySheet;

// CGroundEditDlg dialog

class CMatAnimDlg : public CPropertyPage
{
public:
	CMatAnimDlg(UINT id, Material *m, CMaterialPropertySheet *editor) : _id(id),
		_material(m), CPropertyPage(id), _editor(editor)		{}   // standard constructor
	virtual ~CMatAnimDlg()							{}

protected:

	CMaterialPropertySheet*			_editor;
	Material*						_material;
	UINT							_id;

	int								getMaterialAnimation(const string& animName) const;

public:

	UINT							getIDD() const				{ return _id; }
};
