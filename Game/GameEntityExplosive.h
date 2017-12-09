
#ifndef _GAMEENTITYEXPLOSIVE_H_
#define _GAMEENTITYEXPLOSIVE_H_

#include "GameEntityDynamic.h"
#include "EntityExplosive.h"

class GameEntityExplosive : public GameEntityDynamic
{
protected:

	float				_damage;
	bool				_hasExploded;

public:

	GameEntityExplosive(EntityExplosive& e, const Matrix4& world);

	void				onHit(const Hit& h);
};

#endif