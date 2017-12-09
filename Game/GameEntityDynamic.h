
#ifndef _GAMEENTITYDYNAMIC_H_
#define _GAMEENTITYDYNAMIC_H_

#include "GameEntity.h"
#include "EntityDynamic.h"
#include "Hit.h"

class GameEntityDynamic : public GameEntity
{
public:

	GameEntityDynamic(EntityDynamic& t, const Matrix4& world);

	void				update(float time);
	virtual void		onHit(const Hit& hit);
};

#endif