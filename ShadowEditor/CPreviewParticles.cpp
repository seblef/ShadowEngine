
#include "stdafx.h"
#include "CPreviewParticles.h"
#include <GameLib.h>


IMPLEMENT_DYNAMIC(CPreviewParticles, CStatic)

CPreviewParticles::CPreviewParticles(IMultiVideoDevice* device) : _dragging(false), _lastX(0), _lastY(0),
_device(device), _view(0), _system(0), _refreshing(false)
{
	_cBuffer = device->createConstantBuffer(4, 0);
	_cam.center(BBox3(Vector3(-1,-1,-1),Vector3::UnitVector));
}

CPreviewParticles::~CPreviewParticles()
{
	delete _cBuffer;
	if (_view)				delete _view;
}

BEGIN_MESSAGE_MAP(CPreviewParticles, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



void CPreviewParticles::OnLButtonDown(UINT nFlags, CPoint point)
{
	_dragging = true;
	_lastX = point.x;
	_lastY = point.y;

	__super::OnLButtonDown(nFlags, point);
}


void CPreviewParticles::OnLButtonUp(UINT nFlags, CPoint point)
{
	_dragging = false;
	__super::OnLButtonUp(nFlags, point);
}


void CPreviewParticles::OnMouseMove(UINT nFlags, CPoint point)
{
	if (_dragging)
	{
		float dx = (float)(point.x - _lastX);
		float dy = (float)(point.y - _lastY);

		_cam.rotate(Vector2(dy, dx));
		update(0);

		_lastX = point.x;
		_lastY = point.y;
	}

	__super::OnMouseMove(nFlags, point);
}

void CPreviewParticles::initDialog()
{
	RECT r;
	GetWindowRect(&r);
	ClientToScreen(&r);

	_view = _device->createView(r.right - r.left, r.bottom - r.top, (void*)m_hWnd);
	_cam.onResize(r.right - r.left, r.bottom - r.top);

	update(0);
}

void CPreviewParticles::update(float time)
{
	if (!_view || _refreshing)			return;

	_refreshing = true;

	_device->setCurrentView(_view);
	_device->resetRenderTargets();
	_device->clearRenderTargets(Color::Black);
	_device->clearDepthStencil();

	if (_system)
	{
		_system->update(time, _cam.getCamera());

//		if (_system->getWorldBBox().getSize().getLength() > 0.001f)
//			_cam.getCamera().center(_system->getWorldBBox());


		_cBuffer->set(ST_VERTEX);

		_cam.getCamera().buildMatrices();
		_cBuffer->fill(&_cam.getCamera().getViewProjMatrix());

		ParticleEngine::getSingletonRef().enqueueSystem(_system);
		ParticleEngine::getSingletonRef().draw(_cam.getCamera());
	}

	_device->newFrame();

	_refreshing = false;
}
