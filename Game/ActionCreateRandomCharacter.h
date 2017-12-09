
#ifndef _ACTIONCREATERANDOMCHARACTER_H_
#define _ACTIONCREATERANDOMCHARACTER_H_

#include "Action.h"
#include <Core.h>

using namespace Core;

class ActionCreateRandomCharacter : public Action
{
protected:

	void				findPosAndOrientation(Vector2& pos, float& orientation) const;

public:

	ActionCreateRandomCharacter()			{}

	void				update(float time);
};

#endif