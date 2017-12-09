
#pragma once

#define RI_MATERIAL			0
#define RI_MESH				1
#define RI_ACTOR			2
#define RI_PARTICLES		3
#define RI_SOUND			4
#define RI_GAMEMATERIAL		5
#define RI_ENTITY			6

/////////////////////////////////////////////////////////////////////////////
// CViewTree window

class CResourceTree : public CTreeCtrl
{
// Construction
public:
	CResourceTree();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CResourceTree();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	DECLARE_MESSAGE_MAP()
};
