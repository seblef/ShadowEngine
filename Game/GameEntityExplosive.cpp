
#include "stdafx.h"
#include "GameEntityExplosive.h"
#include "ActionServer.h"
#include "Explosion.h"

GameEntityExplosive::GameEntityExplosive(EntityExplosive& e, const Matrix4& world) : GameEntityDynamic(e, world), _damage(0),
	_hasExploded(false)
{

}

void GameEntityExplosive::onHit(const Hit& h)
{
	GameEntityDynamic::onHit(h);

	if (_hasExploded)
		return;

	_damage += h._damage;
	EntityExplosive* e = (EntityExplosive*)_template;

	if (_damage >= e->getDamageThreshold())
	{
		_damage = 0;
		Vector3 pos=_world;
		ActionServer::getSingletonRef().addDelayedAction(new Explosion(*e->getExplosion(), pos),0.1f);
		_hasExploded = true;
	}
}