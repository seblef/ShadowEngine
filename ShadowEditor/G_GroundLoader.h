
#ifndef _G_GROUNDLOADER_H_
#define _G_GROUNDLOADER_H_

#include "G_Ground.h"
#include "G_Map.h"

class G_GroundLoader
{
public:

	G_GroundLoader()				{}

	void							loadGround(const string& groundFile, G_Ground& g,
		G_Map& m) const;
};

#endif