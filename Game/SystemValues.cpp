
#include "stdafx.h"
#include "SystemValues.h"

void SystemValues::parseConfig(Config& c)
{
	c.getVar("showFPS", _showFPS);
	c.getVar("hudFont", _hudFont);
	c.getVar("crossTexture", _crossTexture);
	c.getVar("crossSize", _crossSize);
	c.getVar("cameraBaseDistance", _cameraBaseDistance);
	c.getVar("gravity", _gravity);
	c.getVar("baseDebrisCount", _baseDebrisCount);
	c.getVar("baseDebrisMinVelocity", _baseDebrisMinVelocity);
	c.getVar("baseDebrisMaxVelocity", _baseDebrisMaxVelocity);
	c.getVar("baseDebrisAngle", _baseDebrisAngle);
	c.getVar("baseDebrisAngleSpeed", _baseDebrisAngleSpeed);
	c.getVar("baseDebrisSize", _baseDebrisSize);
	c.getVar("aiDamageFactor", _aiDamageFactor);
	c.getVar("playerDamageFactor", _playerDamageFactor);
	c.getVar("damageToForce", _damageToForce);
	c.getVar("deadActorStayOnMap", _deadActorStayOnMapTime);
}