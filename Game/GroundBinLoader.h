
#ifndef _GROUNDBINLOADER_H_
#define _GROUNDBINLOADER_H_

#include "Ground.h"
#include "Map.h"

class GroundBinLoader
{
public:

	GroundBinLoader()				{}

	void							loadGround(Ground& g, const string& groundFile, Map& mp) const;
};

#endif