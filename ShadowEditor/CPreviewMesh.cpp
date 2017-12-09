
#include "stdafx.h"
#include "CPreviewMesh.h"
#include <GameLib.h>


IMPLEMENT_DYNAMIC(CPreviewMesh, CStatic)

CPreviewMesh::CPreviewMesh(Material *mat, Geometry* geo, IMultiVideoDevice* device) : _dragging(false), _lastX(0), _lastY(0),
	_device(device), _view(0), _mat(mat), _refreshing(false), _geo(geo)
{
	_shader=device->createShader("Editor/MaterialEd");
	_backShader=device->createShader("Editor/Back");
	_backTexture=device->createTexture("Textures/Editor/MaterialBack.dds");
	_cBuffer=device->createConstantBuffer(7,0);
	_backBlendState=device->createBlendState(false,BLEND_ONE,BLEND_ZERO);
	_backDepthState=device->createDepthStencilState(false,false,COMP_ALWAYS);
	_depthState=device->createDepthStencilState(true,true,COMP_LESSEQUAL);

	_cam.center(geo->getBBox());
	_sphereVBuf=device->createVertexBuffer(geo->getVertexCount(),VX_3D,BU_IMMUTABLE,geo->getVertices());
	_sphereIBuf=device->createIndexBuffer(geo->getTriangleCount()*3,IDX_16BITS,BU_IMMUTABLE,geo->getTriangles());
}

CPreviewMesh::~CPreviewMesh()
{
	delete _shader;
	delete _backShader;
	delete _backTexture;
	delete _cBuffer;
	_device->destroyBlendState(_backBlendState);
	_device->destroyDepthStencilState(_depthState);
	_device->destroyDepthStencilState(_backDepthState);

	if(_sphereVBuf)			delete _sphereVBuf;
	if(_sphereIBuf)			delete _sphereIBuf;

	if(_view)				delete _view;
}

BEGIN_MESSAGE_MAP(CPreviewMesh, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



void CPreviewMesh::OnLButtonDown(UINT nFlags, CPoint point)
{
	_dragging=true;
	_lastX=point.x;
	_lastY=point.y;

	__super::OnLButtonDown(nFlags, point);
}


void CPreviewMesh::OnLButtonUp(UINT nFlags, CPoint point)
{
	_dragging=false;
	__super::OnLButtonUp(nFlags, point);
}


void CPreviewMesh::OnMouseMove(UINT nFlags, CPoint point)
{
	if(_dragging)
	{
		float dx=(float)(point.x - _lastX);
		float dy=(float)(point.y - _lastY);

		_cam.rotate(Vector2(dy,dx));
		update(0);

		_lastX=point.x;
		_lastY=point.y;
	}

	__super::OnMouseMove(nFlags, point);
}

void CPreviewMesh::initDialog()
{
	RECT r;
	GetWindowRect(&r);
	ClientToScreen(&r);

	_view=_device->createView(r.right-r.left,r.bottom-r.top,(void*)m_hWnd);
	_cam.onResize(r.right-r.left,r.bottom-r.top);

	update(0);
}

void CPreviewMesh::update(float time)
{
	if(!_view || _refreshing)			return;

	_refreshing=true;

	_device->setCurrentView(_view);

	MaterialSystem::getSingletonRef().setBuffers();
	_device->resetRenderTargets();
	_device->clearDepthStencil();

	_backShader->set();
	_backTexture->set(0);
	_device->setBlendState(_backBlendState);
	_device->setDepthStencilState(_backDepthState);
	_device->renderFullscreenQuad();

	_device->setInputLayer(VX_3D);
	_cBuffer->set(ST_PIXEL);
	_cBuffer->set(ST_VERTEX);

	_sphereVBuf->set();
	_sphereIBuf->set();
	_device->setDepthStencilState(_depthState);
	_shader->set();

	struct
	{
		Matrix4				viewProj;
		Vector4				eyePos;
		Vector4				lightDir;
		Vector4				lightColor;
	} buffer;

	_cam.getCamera().buildMatrices();
	buffer.viewProj=_cam.getCamera().getViewProjMatrix();
	buffer.lightDir=Vector4(1,-1,1,0);
	buffer.lightDir.normalize();
	buffer.lightColor=Vector4(0.8f,0.8f,0.8f,1.0f);
	buffer.eyePos=_cam.getCamera().getPosition();

	_cBuffer->fill(&buffer);

	_mat->setBase(time);

	_device->renderIndexed(_sphereIBuf->getIndexCount());

//	_mat->setAdd();

//	_device->renderIndexed(_sphereIBuf->getIndexCount());
	_device->newFrame();

	_refreshing=false;
}
