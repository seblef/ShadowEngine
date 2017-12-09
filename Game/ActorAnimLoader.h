
#ifndef _ACTORANIMLOADER_H_
#define _ACTORANIMLOADER_H_

#include <StdRenderer.h>

class ActorAnimLoader
{
public:

	static AnimPRSMultiTrack*			loadActorAnimation(const string& animFile);
};

#endif