
#ifndef _MATERIALANIMATIONFACTORY_H_
#define _MATERIALANIMATIONFACTORY_H_

#include "MaterialAnimation.h"

class MaterialAnimationFactory
{
public:

	MaterialAnimationFactory()			{}

	static MaterialAnimation*			createAnimation(const string& animName, Material *m);
	static MaterialAnimation*			parseAnimation(ScriptFile& sf, Material *m);
	static MaterialAnimation*			copyAnimation(const MaterialAnimation* a);
};

#endif