
#ifndef _ENTITYUPDATER_H_
#define _ENTITYUPDATER_H_

#include "GameEntity.h"

class EntityUpdater : public TSingleton<EntityUpdater>
{
protected:

	typedef set<GameEntity*>	EntitySet;
	EntitySet					_entities;

public:

	EntityUpdater()				{}

	void						addEntity(GameEntity* e)			{ _entities.insert(e); }
	void						remEntity(GameEntity* e)			{ _entities.erase(e); }

	void						updateEntities(float time) const;

	void						clear()								{ _entities.clear(); }
};

#endif