
#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include <Core.h>

using namespace Core;

class GameEntityTrigger;
class GameCharacter;

#define TRIGGER_REBIRTH_DELAY				5.0f

class Trigger
{
public:

	Trigger()			{}
	virtual ~Trigger()	{}

	virtual void		apply(GameCharacter* actor, GameEntityTrigger* entity) = 0;

	virtual void		load()						{}
	virtual void		unload()					{}
};

class TriggerFactory
{
public:

	TriggerFactory()			{}

	static Trigger*				createTrigger(const string& className, ScriptFile& sf);
};

#endif