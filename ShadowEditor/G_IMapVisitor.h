
#ifndef _G_IMAPVISITOR_H_
#define _G_IMAPVISITOR_H_

#include "G_Ground.h"
#include "G_Object.h"

class G_IMapVisitor
{
public:

	G_IMapVisitor()				{}
	virtual ~G_IMapVisitor()	{}

	virtual void				visitGround(G_Ground *g)				{}
	virtual void				visitObject(G_Object* o)				{}
};

#endif