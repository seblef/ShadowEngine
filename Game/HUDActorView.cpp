
#include "stdafx.h"
#include "HUDActorView.h"
#include "HUDActor.h"

#define HUDACTORVIEW_MAX_VERTICES				384

static const float		HUDActorPositionOffset = 0.25f;
static const float		HUDActorLifeRectWidth = 0.25f;
static const float		HUDActorLifeRectHeight = 0.05f;
static const float		HUDActorLifeRectAlpha = 0.6f;

static const Color		HUDActorYellowColor = Color(0.0f, 1.0f, 1.0f, HUDActorLifeRectAlpha);
static const Color		HUDActorGreenColor = Color(0.0f, 1.0f, 0.0f, HUDActorLifeRectAlpha);
static const Color		HUDActorRedColor = Color(1.0f, 0.0f, 0.0f, HUDActorLifeRectAlpha);

extern ofstream			g_MediaOut;

HUDActorView::HUDActorView(IVideoDevice *device) : _device(device),
	_vertices(128)
{
	_vertices.clear();
	_shader = device->createShader("HUD/Actor");
	_vBuffer = device->createVertexBuffer(HUDACTORVIEW_MAX_VERTICES, VX_BASE,
		BU_DYNAMIC);
	_blendState = device->createBlendState(true, BLEND_SRCALPHA, BLEND_INVSRCALPHA);
	_renderState = device->createRenderState(CULL_NONE);
	_dsState = device->createDepthStencilState(true, false, COMP_LESSEQUAL);
}

HUDActorView::~HUDActorView()
{
	delete _shader;
	delete _vBuffer;

	_device->destroyBlendState(_blendState);
	_device->destroyRenderState(_renderState);
	_device->destroyDepthStencilState(_dsState);
}

void HUDActorView::renderHUD(Camera& c, const set<HUDActor*>& actors)
{
	Vector3 xAxis(c.getXAxis());
	Vector3 yAxis(c.getYAxis());

	xAxis *= HUDActorLifeRectWidth*0.5f;
	yAxis *= HUDActorLifeRectHeight*0.5f;

	VxBase v[6];
	Vector3 pos;
	Color col;
	Vector3 xsize;
	float lifeRatio;

	set<HUDActor*>::const_iterator a(actors.begin());
	const HUDActor* actor;
	for (; a != actors.end(); ++a)
	{
		actor = *a;
		lifeRatio = actor->getCurrentLife() / actor->getMaxLife();
		pos = actor->getPosition();
		pos.y += actor->getActorHeight() + HUDActorPositionOffset;

		if (lifeRatio >= 0.5f)
		{
			AnimVector4Evaluator::evaluate(HUDActorYellowColor, HUDActorGreenColor,
				(lifeRatio - 0.5f) * 2.0f, col);
//			g_MediaOut << "Life ratio >= 0.5 (" << lifeRatio << "), color: " << col.r << " , " << col.g << " , " << col.b << endl;
		}
		else
		{
			AnimVector4Evaluator::evaluate(HUDActorRedColor, HUDActorYellowColor,
				lifeRatio * 2.0f, col);
//			g_MediaOut << "Life ratio < 0.5 (" << lifeRatio << "), color: " << col.r << " , " << col.g << " , " << col.b << endl;
		}

		for (int i = 0; i < 6; ++i)
		{
			v[i].color = col;
			v[i].pos = pos;
		}

		xsize = xAxis * (lifeRatio * 2.0f - 1.0f);

		v[0].pos -= xAxis;
		v[0].pos -= yAxis;

		v[1].pos += xsize;
		v[1].pos -= yAxis;

		v[2].pos -= xAxis;
		v[2].pos += yAxis;

		v[3].pos = v[1].pos;
		v[4].pos = v[2].pos;
			
		v[5].pos += xsize;
		v[5].pos += yAxis;

		_vertices.addTab(v, 6);
	}

	int v_count = _vertices.getCount();
	if (v_count > 0)
	{
		_shader->set();
		_vBuffer->set();
		_device->resetRenderTargets();
		_device->setBlendState(_blendState);
		_device->setDepthStencilState(_dsState);
		_device->setRenderState(_renderState);
		_device->setInputLayer(VX_BASE);
		_device->setPrimitiveType(PT_TRIANGLELIST);

		int count;

		while (v_count > 0)
		{
			count = smin(v_count, HUDACTORVIEW_MAX_VERTICES);

			_vBuffer->fill(((const VxBase*)_vertices.getBuffer()) + _vertices.getCount() - v_count,
				count);
			_device->render(count);

			v_count -= count;
		}

		_vertices.clear();
	}
}