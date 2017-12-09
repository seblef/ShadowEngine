
#include "stdafx.h"
#include "Light.h"
#include "LightSystem.h"


void Light::enqueue()
{
	LightSystem::getSingletonRef().enqueueLight(this);
}