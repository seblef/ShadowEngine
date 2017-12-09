
#include "stdafx.h"
#include "UpdateSystem.h"

void IUpdatable::wakeUp()
{
	_sleeping=false;
	UpdateSystem::getSingletonRef().registerUpdatable(this);
}

void IUpdatable::sleep()
{
	_sleeping=true;
	UpdateSystem::getSingletonRef().unregisterUpdatable(this);
}