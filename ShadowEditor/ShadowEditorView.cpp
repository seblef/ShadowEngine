
// ShadowEditorView.cpp : implementation of the CShadowEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ShadowEditor.h"
#endif

#include "ShadowEditorDoc.h"
#include "ShadowEditorView.h"

#include "G_EditorApp.h"
#include "G_Camera.h"
#include "G_ToolSystem.h"
#include <StdRenderer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShadowEditorView

IMPLEMENT_DYNCREATE(CShadowEditorView, CView)

BEGIN_MESSAGE_MAP(CShadowEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CShadowEditorView construction/destruction

CShadowEditorView::CShadowEditorView() : _view(0), _dragging(false)
{
	// TODO: add construction code here

}

CShadowEditorView::~CShadowEditorView()
{
}

BOOL CShadowEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CShadowEditorView drawing

void CShadowEditorView::OnDraw(CDC* /*pDC*/)
{
	G_EditorApp::getSingletonRef().refresh();
}

void CShadowEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);

	if(G_ToolSystem::getSingletonRef().isCurrentToolNull())
		OnContextMenu(this, point);
	else if(_dragging)
		G_ToolSystem::getSingletonRef().cancel();
	else
		G_ToolSystem::getSingletonRef().setCurrentTool(GBT_NULL);
}

void CShadowEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CShadowEditorView diagnostics

#ifdef _DEBUG
void CShadowEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CShadowEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShadowEditorDoc* CShadowEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShadowEditorDoc)));
	return (CShadowEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CShadowEditorView message handlers


int CShadowEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CShadowEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(cx > 0 && cy > 0)
	{
		if(_view)
			G_EditorApp::getSingletonRef().getVideo()->resizeView(_view,cx,cy);
		else
		{
			_view=G_EditorApp::getSingletonRef().getVideo()->createView(cx,cy,(void*)this->m_hWnd);
			G_EditorApp::getSingletonRef().setMainView(_view);
		}

		Renderer::getSingletonRef().onResize(cx,cy);
		G_Camera::getSingletonRef().onResize(cx,cy);
		G_EditorApp::getSingletonRef().refresh();
	}
}


void CShadowEditorView::OnDestroy()
{
	CView::OnDestroy();

	if(_view)			delete _view;
}


void CShadowEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	_dragging=true;
	if(nFlags & MK_CONTROL) 		G_ToolSystem::getSingletonRef().setFlags(GTOOL_CTRLDOWN);
	else							G_ToolSystem::getSingletonRef().unsetFlags(GTOOL_CTRLDOWN);

	_lastMouseX=point.x;
	_lastMouseY=point.y;

	G_ToolSystem::getSingletonRef().mouseDown(point.x,point.y);

	CView::OnLButtonDown(nFlags, point);
}


void CShadowEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	_dragging=false;
	G_ToolSystem::getSingletonRef().mouseUp(point.x,point.y);

	CView::OnLButtonUp(nFlags, point);
}


void CShadowEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(_dragging)
	{
		G_ToolSystem::getSingletonRef().mouseMove(point.x,point.y);
		_lastMouseX=point.x;
		_lastMouseY=point.y;
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CShadowEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	G_Camera::getSingletonRef().scaleDist(zDelta < 0 ? 1.1f : 1.0f / 1.1f);
	G_EditorApp::getSingletonRef().refresh();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CShadowEditorView::OnMouseLeave()
{
	if(_dragging)
	{
		_dragging=false;
		G_ToolSystem::getSingletonRef().mouseUp(_lastMouseX,_lastMouseY);
	}

	CView::OnMouseLeave();
}
