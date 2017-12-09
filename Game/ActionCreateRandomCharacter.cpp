
#include "stdafx.h"
#include "ActionCreateRandomCharacter.h"
#include "GameSystem.h"
#include "GameCharacter.h"
#include "Map.h"
#include "Ground.h"
#include <NavigationLib.h>
#include <PhysicLib.h>

void ActionCreateRandomCharacter::update(float time)
{
	_dead = true;

	Vector2 pos;
	float orientation;
	findPosAndOrientation(pos, orientation);

	const map<string, Character*>& db(GameSystem::getSingletonRef().getCharacterDB().getData());

	int char_count = db.size();
	int char_id = rand() % char_count;
	map<string, Character*>::const_iterator c(db.begin());

	for (int i = 0; i < char_id; ++i)
		++c;

	GameCharacter* actor = new GameCharacter(*c->second, pos, orientation, true);
	GameSystem::getSingletonRef().getMap().addObject(actor, true);
}

void ActionCreateRandomCharacter::findPosAndOrientation(Vector2& pos, float& orientation) const
{
	bool found = false;
	int width = GameSystem::getSingletonRef().getMap().getGround().getWidth();
	int height = GameSystem::getSingletonRef().getMap().getGround().getHeight();
	int x, y;
	const NavMap& nm(Navigation::getSingletonRef().getMap());

	while (!found)
	{
		x = rand() % width;
		y = rand() % height;

		if (nm.getFlag(x, y, NAV_WAKABLE))
			found = true;
	};

	pos.x = 0.5f + (float)x;
	pos.y = 0.5f + (float)y;

	RayCastInfos rc;
	rc._dir.y = 0.0f;
	rc._origin.x = pos.x;
	rc._origin.y = 1.0f;
	rc._origin.z = pos.y;
	rc._range = 10.0f;

	for (float o = -180.0f; o < 180.0f; o += 45.0f)
	{
		rc._dir.x = sinf(o * M_PI / 180.0f);
		rc._dir.z = cosf(o * M_PI / 180.0f);
		if (!Physic::getSingletonRef().testRay(rc))
		{
			orientation = o;
			break;
		}
	}
}