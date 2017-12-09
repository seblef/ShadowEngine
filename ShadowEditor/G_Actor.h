
#ifndef _G_ACTOR_H_
#define _G_ACTOR_H_

#include <GameLib.h>
#include "G_Object.h"

class G_Actor : public G_Object
{
protected:

	ActorInstance*			_rActor;
	Character*				_template;

	void					rebuildMatrix();

public:

	G_Actor(Character *c);
	G_Actor(const G_Actor& c);
	~G_Actor()
	{
		delete _rActor;
	}

	Character*					getTemplate() const				{ return _template; }

	virtual void				onAddToScene();
	virtual void				onRemFromScene();

	G_Object*					copy()								{ return new G_Actor(*this); }

};

#endif