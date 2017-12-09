
#ifndef _GAMEACTORUPDATER_H_
#define _GAMEACTORUPDATER_H_

#include "GameCharacter.h"

class GameActorUpdater : public TSingleton<GameActorUpdater>
{
protected:

	typedef set<GameCharacter*>		ActorSet;
	ActorSet						_actors;

public:

	GameActorUpdater()				{}
	~GameActorUpdater()				{}

	void							addActor(GameCharacter* c)			{ _actors.insert(c); }
	void							remActor(GameCharacter* c)			{ _actors.erase(c); }

	void							updateActors(float time) const
	{
		ActorSet::const_iterator a(_actors.begin());
		for (; a != _actors.end(); ++a)
			(*a)->update(time);
	}
};

#endif