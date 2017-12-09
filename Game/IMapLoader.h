
#ifndef _IMAPLOADER_H_
#define _IMAPLOADER_H_

#include "Map.h"
#include <string>


class IMapLoader
{
public:

	IMapLoader()				{}
	virtual ~IMapLoader()		{}

	virtual Map*				loadMap(const string& mapName)=0;
};

#endif