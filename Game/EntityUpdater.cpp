
#include "stdafx.h"
#include "EntityUpdater.h"

void EntityUpdater::updateEntities(float time) const
{
	EntitySet::const_iterator e(_entities.begin());
	for (; e != _entities.end(); ++e)
		(*e)->update(time);
}
