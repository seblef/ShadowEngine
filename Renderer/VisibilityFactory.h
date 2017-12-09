
#ifndef _VISIBILITYFACTORY_H_
#define _VISIBILITYFACTORY_H_

#include "IVisibilitySystem.h"

class VisibilityFactory
{
public:

	VisibilityFactory()				{}
	~VisibilityFactory()			{}

	static IVisibilitySystem*		createVisibilitySystem(const string& name);
};

#endif