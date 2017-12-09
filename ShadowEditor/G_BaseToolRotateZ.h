
#ifndef _G_BASETOOLROTATEZ_H_
#define _G_BASETOOLROTATEZ_H_

#include "G_BaseToolRotate.h"
#include "G_Object.h"


class G_BaseToolRotateZ : public G_BaseToolRotate
{
public:

	G_BaseToolRotateZ()		{}

	virtual void		mouseMove(int x, int y);
};

#endif