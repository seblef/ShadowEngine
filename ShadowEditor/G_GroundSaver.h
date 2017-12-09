
#ifndef _G_GROUNDSAVER_H_
#define _G_GROUNDSAVER_H_

#include "G_Ground.h"

class G_GroundSaver
{
public:

	G_GroundSaver()				{}

	void						saveGround(G_Ground& g, const string& fileName) const;
};

#endif