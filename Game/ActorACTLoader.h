
#ifndef _ACTORACTLOADER_H_
#define _ACTORACTLOADER_H_

#include "IActorLoader.h"

class ActorACTLoader : public IActorLoader
{
public:

	ActorACTLoader()			{}
	~ActorACTLoader()			{}

	Actor*						loadActor(const string& actorFile, Material* mat, IVideoDevice* device);
};

#endif