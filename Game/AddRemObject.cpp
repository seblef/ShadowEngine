
#include "stdafx.h"
#include "AddRemObject.h"
#include "GameSystem.h"
#include "Map.h"

void AddRemObject::update(float time)
{
	if (_object)
	{
		if (_remove)
		{
			GameSystem::getSingletonRef().getMap().remObject(_object);
			if (_delete)
				delete _object;
		}
		else
			GameSystem::getSingletonRef().getMap().addObject(_object,true);

		_object = 0;
	}

	_dead = true;
}