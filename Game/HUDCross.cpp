
#include "stdafx.h"
#include "HUDCross.h"
#include "GameSystem.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "SystemValues.h"
#include <PhysicLib.h>


HUDCross::HUDCross(R2D_Layer* hudLayer)
{
	_cross = new R2D_Image(Vector2::NullVector,Vector2::UnitVector,
		Color::White,Vector2::NullVector, Vector2::UnitVector,
		Renderer2D::getSingletonRef().getDevice()->createTexture(SystemValues::getSingletonRef().getCrossTexture()),
		true);

	hudLayer->getSet().insert(_cross);
}

void HUDCross::updateCross()
{
	WeaponInstance* wi = GameSystem::getSingletonRef().getPlayer().getWeapon();
	
	const Matrix4& weaponWorld(wi->getMeshInstance()->getWorldMatrix());

	RayCastInfos rc;
	rc._origin=wi->getTemplate()->getFirePoint() * weaponWorld;
	rc._dir=Vector3::ZAxisVector;
	rc._dir.transformNoTranslation(weaponWorld);
	rc._impact=rc._origin + rc._dir * wi->getTemplate()->getRange();
	rc._range = wi->getTemplate()->getRange();
	rc._flags = RAYCAST_SOLID;

	Physic::getSingletonRef().rayCastImpact(rc);

	Vector4 pos2D(rc._impact.x,rc._impact.y, rc._impact.z, 1.0f);
	Camera& cam(GameSystem::getSingletonRef().getCamera().getCamera());
	pos2D *= cam.getViewProjMatrix();
	pos2D /= pos2D.w;

	Vector2 crossPos;
	Vector2 crossSize;

	if (pos2D.z > 0)
	{
		crossPos.x = (pos2D.x + 1.0f) * cam.getCenter().x;
		crossPos.y = (1.0f - pos2D.y) * cam.getCenter().y;
	}
	else
	{
		crossPos.x = crossPos.y = -100.0f;
	}

	crossSize.x = crossSize.y = SystemValues::getSingletonRef().getCrossSize();
	crossPos -= crossSize*0.5f;

	_cross->setPosition(crossPos);
	_cross->setSize(crossSize);
}