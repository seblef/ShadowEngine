
#ifndef _IACTORLOADER_H_
#define _IACTORLOADER_H_

#include <StdRenderer.h>

class IActorLoader
{
public:

	IActorLoader()				{}
	virtual ~IActorLoader()		{}

	virtual Actor*				loadActor(const string& actorFile, Material* mat, IVideoDevice *device)=0;
};

#endif