
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ShadowEditor.h"
#include "MapPropertiesDlg.h"
#include "G_EditorApp.h"
#include "G_ToolSystem.h"
#include "G_MapSaver.h"
#include "G_MapLoader.h"
#include "G_Selection.h"
#include "G_EditorDlgFactory.h"
#include "G_Creator.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HINSTANCE			g_AppHInstance = 0;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_COMMAND(ID_FILE_DELETE, &CMainFrame::OnFileDelete)
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE2, &CMainFrame::OnFileSave)
	ON_COMMAND(ID_TOOL_ALIGN, &CMainFrame::OnToolAlign)
	ON_COMMAND(ID_TOOL_CAMROTATE, &CMainFrame::OnToolCamrotate)
	ON_COMMAND(ID_TOOL_CAMTRANSLATE, &CMainFrame::OnToolCamtranslate)
	ON_COMMAND(ID_TOOL_FALL2, &CMainFrame::OnToolFall2)
	ON_COMMAND(ID_TOOL_LOCKX, &CMainFrame::OnToolLockx)
	ON_COMMAND(ID_TOOL_LOCKY, &CMainFrame::OnToolLocky)
	ON_COMMAND(ID_TOOL_LOCKZ, &CMainFrame::OnToolLockz)
	ON_COMMAND(ID_TOOL_REDO, &CMainFrame::OnToolRedo)
	ON_COMMAND(ID_TOOL_ROTATE, &CMainFrame::OnToolRotate)
	ON_COMMAND(ID_TOOL_SELECT, &CMainFrame::OnToolSelect)
	ON_COMMAND(ID_TOOL_TRANSLATE, &CMainFrame::OnToolTranslate)
	ON_COMMAND(ID_TOOL_UNDO, &CMainFrame::OnToolUndo)
	ON_COMMAND(ID_EDIT_MAPPROPERTIES, &CMainFrame::OnEditMapproperties)
	ON_COMMAND(ID_CREATE_MESH, &CMainFrame::OnCreateMesh)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LOCKX, &CMainFrame::OnUpdateToolLockx)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LOCKY, &CMainFrame::OnUpdateToolLocky)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LOCKZ, &CMainFrame::OnUpdateToolLockz)
	ON_COMMAND(ID_TOOLS_SNAP, &CMainFrame::OnToolsSnap)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SNAP, &CMainFrame::OnUpdateToolsSnap)
	ON_COMMAND(ID_TOOLS_HEIGHT, &CMainFrame::OnToolsHeight)
	ON_COMMAND(ID_CREATE_LIGHT_OMNI, &CMainFrame::OnCreateLightOmni)
	ON_COMMAND(ID_CREATE_LIGHT_SPOT, &CMainFrame::OnCreateLightSpot)
	ON_COMMAND(ID_CREATE_LIGHT_AREA, &CMainFrame::OnCreateLightArea)
	ON_COMMAND(ID_TOOLS_ROTATEZ, &CMainFrame::OnToolsRotatez)
END_MESSAGE_MAP()


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	g_AppHInstance = ::AfxGetApp()->m_hInstance;
	new G_EditorApp(::AfxGetApp()->m_hInstance);

	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndMenuBar.Create(this);
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar.LoadToolBar(IDR_MAINBAR);
	m_wndToolBar.SetWindowText("Tools");
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, "Customize");

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	m_wndStatusBar.Create(this);

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	CreateDockingWindows();

	_resView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&_resView);
	CDockablePane* pTabbedBar = NULL;
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	_editorView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&_editorView);

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, "Customize", ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		if(m_UserImages.Load(_T(".\\UserImages.bmp")))
			CMFCToolBar::SetUserImages(&m_UserImages);
	}

	for(int i=0;i<m_wndToolBar.GetCount();++i)
	{
		UINT uID,uStyle;
		int iImg;
		m_wndToolBar.GetButtonInfo(i,uID,uStyle,iImg);

		if(uID==ID_TOOLS_LOCKX || uID==ID_TOOLS_LOCKY || uID==ID_TOOLS_LOCKZ || uID==ID_TOOLS_SNAP)
		{
			uStyle|=TBBS_CHECKBOX;
			m_wndToolBar.SetButtonStyle(i,uStyle);
		}
	}

	_editorView.setEditor(G_EditorDlgFactory::createEditorDlg(0,false,&_editorView));

	return 0;
}

BOOL CMainFrame::CreateDockingWindows()
{
	DWORD style=WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI;
	_resView.Create("Resources", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, style);
	m_wndOutput.Create("Output", this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, style);
	_editorView.Create("Properties", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, style);

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_resView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	_editorView.SetIcon(hClassViewIcon, FALSE);
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
		return 0;

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, "Customize");
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
		return FALSE;

	// enable customization button for all user toolbars
	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, "Customize");
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnEditCopy()
{
	G_Selection::getSingletonRef().saveSelection();
}


void CMainFrame::OnEditPaste()
{
	if(!G_Selection::getSingletonRef().getSavedSelection().empty())
	{
		G_Selection::getSingletonRef().copySavedSelection();
		G_ToolSystem::getSingletonRef().setCurrentTool(GBT_PASTE);
	}
}


void CMainFrame::OnFileDelete()
{
	G_Selection::getSingletonRef().deleteSelection();
	G_Selection::getSingletonRef().clearSavedSelection();
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}


void CMainFrame::OnFileNew()
{
	G_EditorApp::getSingletonRef().newMap();
	_resView.refreshResources();
}


void CMainFrame::OnFileOpen()
{
	FileSystemFactory::getFileSystem()->resetWorkingDirectory();

	CFileDialog file_dlg(TRUE,_T("*.txt"),0,OFN_EXPLORER,_T("Shadow map (*.txt)|*.txt||"));

	if(file_dlg.DoModal()==IDCANCEL)
	{
		FileSystemFactory::getFileSystem()->resetWorkingDirectory();
		return;
	}

	FileSystemFactory::getFileSystem()->resetWorkingDirectory();
	string fullName(file_dlg.GetPathName());
	string shadowName;
	string matName;

	FileSystemFactory::getFileSystem()->extractLocalFileName(fullName,shadowName);
	G_EditorApp::getSingletonRef().loadMap(shadowName);

	SetWindowText(G_EditorApp::getSingletonRef().getMap().getName().c_str());

	_resView.refreshResources();
}


void CMainFrame::OnFileSave()
{
	G_MapSaver saver(&G_EditorApp::getSingletonRef().getMap());
	saver.saveMap();
}


void CMainFrame::OnToolAlign()
{
	// TODO: Add your command handler code here
}


void CMainFrame::OnToolCamrotate()
{
	_editorView.getDialog()->onToolChange();
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CAMROTATE);
}


void CMainFrame::OnToolCamtranslate()
{
	_editorView.getDialog()->onToolChange();
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CAMTRANSLATE);
}


void CMainFrame::OnToolFall2()
{
	Vector3 p;

	const G_Selection::SelectionSet& s(G_Selection::getSingletonRef().getSelection());
	G_Selection::SelectionSet::const_iterator o(s.begin());
	for(;o!=s.end();++o)
	{
		p=(*o)->getPosition();
		p.y=-(*o)->getLocalBox().getMin().y;

		(*o)->setPosition(p);
	}

	G_EditorApp::getSingletonRef().refresh();
}


void CMainFrame::OnToolLockx()
{
	if(G_ToolSystem::getSingletonRef().getFlags(GTOOL_LOCKX))
		G_ToolSystem::getSingletonRef().unsetFlags(GTOOL_LOCKX);
	else
		G_ToolSystem::getSingletonRef().setFlags(GTOOL_LOCKX);
}


void CMainFrame::OnToolLocky()
{
	if(G_ToolSystem::getSingletonRef().getFlags(GTOOL_LOCKY))
		G_ToolSystem::getSingletonRef().unsetFlags(GTOOL_LOCKY);
	else
		G_ToolSystem::getSingletonRef().setFlags(GTOOL_LOCKY);
}


void CMainFrame::OnToolLockz()
{
	if(G_ToolSystem::getSingletonRef().getFlags(GTOOL_LOCKZ))
		G_ToolSystem::getSingletonRef().unsetFlags(GTOOL_LOCKZ);
	else
		G_ToolSystem::getSingletonRef().setFlags(GTOOL_LOCKZ);
}


void CMainFrame::OnToolRedo()
{
	// TODO: Add your command handler code here
}


void CMainFrame::OnToolRotate()
{
	_editorView.getDialog()->onToolChange();
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_ROTATE);
}


void CMainFrame::OnToolSelect()
{
	_editorView.getDialog()->onToolChange();
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}


void CMainFrame::OnToolTranslate()
{
	_editorView.getDialog()->onToolChange();
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_TRANSLATE);
}


void CMainFrame::OnToolUndo()
{
	// TODO: Add your command handler code here
}


void CMainFrame::OnEditMapproperties()
{
	G_Map& m=G_EditorApp::getSingletonRef().getMap();

	CMapPropertiesDlg dlg(&m,false);
	if(dlg.DoModal()==IDCANCEL)
		return;

	if(m.getGround().getWidth()!=dlg.getWidth() || m.getGround().getHeight()!=dlg.getHeight())
		m.getGround().resize(dlg.getWidth(),dlg.getHeight());

	m.setName(dlg.getName());

	if(dlg.isSunEnable())
		LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(dlg.getSunColor(),dlg.getSunDirection());
	else
		LightSystem::getSingletonRef().getGlobalLight().disableGlobalLight();

	LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(dlg.getAmbient());

	G_EditorApp::getSingletonRef().refresh();
}

void CMainFrame::onSelectionChange()
{
	G_Object *o=0;
	bool ground=false;

	if(G_Selection::getSingletonRef().isGroundSelected())
		ground=true;
	else if(G_Selection::getSingletonRef().getSelection().size()==1)
		o=*G_Selection::getSingletonRef().getSelection().begin();

	_editorView.setEditor(G_EditorDlgFactory::createEditorDlg(o,ground,&_editorView));
}

void CMainFrame::OnCreateMesh()
{
	G_Creator::createMesh(0);
}


void CMainFrame::OnUpdateToolLockx(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(G_ToolSystem::getSingletonRef().getFlags(GTOOL_LOCKX) ? TRUE : FALSE);
}


void CMainFrame::OnUpdateToolLocky(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(G_ToolSystem::getSingletonRef().getFlags(GTOOL_LOCKY) ? TRUE : FALSE);
}


void CMainFrame::OnUpdateToolLockz(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(G_ToolSystem::getSingletonRef().getFlags(GTOOL_LOCKZ) ? TRUE : FALSE);
}


void CMainFrame::OnToolsSnap()
{
	if(G_ToolSystem::getSingletonRef().getFlags(GTOOL_SNAPGRID))
		G_ToolSystem::getSingletonRef().unsetFlags(GTOOL_SNAPGRID);
	else
		G_ToolSystem::getSingletonRef().setFlags(GTOOL_SNAPGRID);
}


void CMainFrame::OnUpdateToolsSnap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(G_ToolSystem::getSingletonRef().getFlags(GTOOL_SNAPGRID) ? TRUE : FALSE);
}


void CMainFrame::OnToolsHeight()
{
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_HEIGHT);
}


void CMainFrame::OnCreateLightOmni()
{
	G_Creator::createLight(Light::LT_OMNI);
}


void CMainFrame::OnCreateLightSpot()
{
	G_Creator::createLight(Light::LT_SPOT);
}


void CMainFrame::OnCreateLightArea()
{
	G_Creator::createLight(Light::LT_AREA);
}


void CMainFrame::OnToolsRotatez()
{
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_ROTATEZ);
}
