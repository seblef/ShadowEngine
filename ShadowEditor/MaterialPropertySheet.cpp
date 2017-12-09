// MaterialPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MaterialPropertySheet.h"
#include "G_EditorApp.h"
#include "../Game/GeometryLoader.h"


// CMaterialPropertySheet

IMPLEMENT_DYNAMIC(CMaterialPropertySheet, CPropertySheet)

CMaterialPropertySheet::CMaterialPropertySheet(Material* m, const CString& matName,  BOOL editName, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage), _mat(m), _editName(editName),
	_save(m),
	_colorDlg(m,this,matName,editName),
	_textureDlg(m,this),
	_stateDlg(m,this),
	_uvDlg(m,this),
	_uvAnimDlg(m,this),
	_texAnimDlg(m,this),
	_fadeAnimDlg(m,this),
	_preview(m,GeometryLoader::loadGeometry("Editor/sphere.geo"), G_EditorApp::getSingletonRef().getVideo())
{
	AddPage(&_colorDlg);
	AddPage(&_textureDlg);
	AddPage(&_stateDlg);
	AddPage(&_uvDlg);
	AddPage(&_uvAnimDlg);
	AddPage(&_texAnimDlg);
	AddPage(&_fadeAnimDlg);
}

CMaterialPropertySheet::CMaterialPropertySheet(Material* m, const CString& matName,  BOOL editName, LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage), _mat(m), _editName(editName),
	_save(m),
	_colorDlg(m,this,matName,editName),
	_textureDlg(m,this),
	_stateDlg(m,this),
	_uvDlg(m,this),
	_uvAnimDlg(m,this),
	_texAnimDlg(m,this),
	_fadeAnimDlg(m,this),
	_preview(m,GeometryLoader::loadGeometry("Editor/sphere.geo"), G_EditorApp::getSingletonRef().getVideo())
{
	AddPage(&_colorDlg);
	AddPage(&_textureDlg);
	AddPage(&_stateDlg);
	AddPage(&_uvDlg);
	AddPage(&_uvAnimDlg);
	AddPage(&_texAnimDlg);
	AddPage(&_fadeAnimDlg);
}

CMaterialPropertySheet::~CMaterialPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMaterialPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CMaterialPropertySheet message handlers


BOOL CMaterialPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	int _PropSheetButtons[] = {IDOK, IDCANCEL, ID_APPLY_NOW };

	CRect r;
	GetClientRect(&r);

	int previewSize=r.Width();
	int headerHeight=previewSize + 10;

	GetWindowRect(r);
	ScreenToClient(r);

	SetWindowPos (NULL, 0, 0, r.Width(), 
		r.Height() + headerHeight,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	_preview.CreateEx(0,0,0,WS_CHILD | WS_VISIBLE,-1,-1,r.Width(),previewSize,m_hWnd,0,0);
	_preview.initDialog();
    
    HWND hWnd = (HWND)GetTabControl ()->m_hWnd;
    CRect rectOld;
    ::GetWindowRect (hWnd, &rectOld);
    ScreenToClient (&rectOld);
    ::SetWindowPos (hWnd, NULL, rectOld.left, 
		rectOld.top + headerHeight, rectOld.Width (), 
                    rectOld.Height (),
                    SWP_NOZORDER | SWP_NOACTIVATE);

	CPropertyPage *pages[]={
		&_colorDlg,
		&_textureDlg,
		&_stateDlg,
		&_uvDlg,
		&_uvAnimDlg,
		&_texAnimDlg,
		&_fadeAnimDlg
	};

	for(int i=0;i<7;++i)
	{
		hWnd = (HWND)pages[i]->m_hWnd; 

		::GetWindowRect (hWnd, &rectOld);
		ScreenToClient (&rectOld);
		::SetWindowPos (hWnd, NULL, rectOld.left, 
			rectOld.top + headerHeight, 
						rectOld.Width (), rectOld.Height (),
						SWP_NOZORDER | SWP_NOACTIVATE);
	}
    
    
    // move buttons by similar amount
    for (int i = 0; i < sizeof (_PropSheetButtons) / sizeof (int); i++)
    {
        hWnd = ::GetDlgItem (m_hWnd, _PropSheetButtons [i]);
        if (hWnd != NULL)
        {
            ::GetWindowRect (hWnd, &rectOld);
            ScreenToClient (&rectOld);
            ::SetWindowPos (hWnd, NULL, rectOld.left, 
				rectOld.top + headerHeight, 
                            rectOld.Width (), rectOld.Height (),
                            SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
    
    // Enable Apply Now button
    hWnd = ::GetDlgItem(m_hWnd, ID_APPLY_NOW);
    if (hWnd != NULL)
    {
        ::ShowWindow (hWnd, SW_HIDE);
        ::EnableWindow (hWnd, FALSE);
    }
    
    CenterWindow ();

	return bResult;
}


BOOL CMaterialPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
if (CWnd::OnCommand (wParam, lParam))
        return TRUE;
    
    // crack message parameters
    UINT nID = LOWORD(wParam);
    HWND hWndCtrl = (HWND)lParam;
    int nCode = HIWORD(wParam);
    
    // set m_nModalResult to ID of button, whenever button is clicked
    if (hWndCtrl != NULL && nCode == BN_CLICKED)
    {
        if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0) &
            (DLGC_BUTTON|DLGC_DEFPUSHBUTTON))
        {
            LONG lStyle = ::GetWindowLong(hWndCtrl, GWL_STYLE) & 0x0F;
            if (lStyle == BS_PUSHBUTTON || lStyle == BS_DEFPUSHBUTTON ||
                lStyle == BS_USERBUTTON || lStyle == BS_OWNERDRAW)
            {
                if (nID == IDCANCEL)
                {
					_save.restore();
                    m_nModalResult = nID;
                }
            }
        }
    }
    return FALSE;
}
