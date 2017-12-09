
#include "stdafx.h"
#include "HUD.h"
#include "HUDActor.h"
#include "HUDActorView.h"
#include "HUDCross.h"
#include "GameCamera.h"
#include "FPSCounter.h"
#include "SystemValues.h"
#include "GameSystem.h"

extern ofstream g_MediaOut;

HUD::HUD()
{
	_actorView = new HUDActorView(Renderer2D::getSingletonRef().getDevice());
	_frame = new R2D_Frame;
	_hudLayer = new R2D_Layer;

	_cross = new HUDCross(_hudLayer);

	_frame->getVector().push_back(_hudLayer);

	const string& fontName(SystemValues::getSingletonRef().getHUDFont());

	_hudFont = Renderer2D::getSingletonRef().getFontDB().getData(fontName);
	if (!_hudFont)
	{
		_hudFont = R2D_FontLoader::loadFont(fontName, Renderer2D::getSingletonRef().getDevice());
		Renderer2D::getSingletonRef().getFontDB().registerData(fontName, _hudFont);
	}

	_fpsCounter = new FPSCounter("HUD", SystemValues::getSingletonRef().getShowFPS(), _hudFont);

	if (_fpsCounter->getLayer())
		_frame->getVector().push_back(_fpsCounter->getLayer());

	Renderer2D::getSingletonRef().getFrameDB().registerData("HUD", _frame);
	Renderer2D::getSingletonRef().setCurrentFrame(_frame);
}

HUD::~HUD()
{
	_fpsCounter->outputStats(g_MediaOut);

	Renderer2D::getSingletonRef().getFrameDB().unregisterData("HUD");
	delete _frame;
	delete _fpsCounter;
	delete _actorView;
	delete _cross;

	ActorSet::iterator a(_actors.begin());
	for (; a != _actors.end(); ++a)
		delete *a;
}

HUDActor* HUD::createHUDActor(float maxLife, float actorHeight) const
{
	return new HUDActor(maxLife, actorHeight);
}

void HUD::updateHUD(GameCamera& gc)
{
	_fpsCounter->updateCounter();
	_actorView->renderHUD(gc.getCamera(), _actors);
	_cross->updateCross();
}