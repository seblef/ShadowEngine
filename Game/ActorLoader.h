
#ifndef _ACTORLOADER_H_
#define _ACTORLOADER_H_

#include <StdRenderer.h>

class ActorLoader
{
public:

	ActorLoader()				{}

	static Actor*				loadActor(const string& actorFile, Material* mat, IVideoDevice *device);
};

#endif