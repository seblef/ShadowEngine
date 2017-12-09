
#ifndef _G_BASETOOLNULL_H_
#define _G_BASETOOLNULL_H_

#include "G_Tool.h"

class G_BaseToolNull : public G_Tool
{
protected:

public:

	G_BaseToolNull()				{}
	virtual ~G_BaseToolNull()		{}

	virtual void					mouseDown(int x, int y, unsigned short flags);
};

#endif