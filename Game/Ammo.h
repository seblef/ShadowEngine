
#ifndef _AMMO_H_
#define _AMMO_H_

#include "Action.h"

class IPhysicObject;

class Ammo : public Action
{
public:

	Ammo()			{}
	virtual ~Ammo()	{}

	virtual void	onContact(IPhysicObject* other)			{}
};

#endif