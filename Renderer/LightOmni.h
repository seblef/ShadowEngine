
#ifndef _LIGHTOMNI_H_
#define _LIGHTOMNI_H_

#include "Light.h"

class LightOmni : public Light
{
public:

	LightOmni(const LightCreate_t& c) : Light(LT_OMNI,c)				{}
	LightOmni(const LightOmni* l) : Light(l)							{}
};

#endif