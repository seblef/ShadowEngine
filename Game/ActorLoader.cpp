
#include "stdafx.h"
#include "ActorLoader.h"
#include "ActorACTLoader.h"

Actor* ActorLoader::loadActor(const string& actorFile, Material* mat, IVideoDevice *device)
{
	IActorLoader *loader=0;
	Actor* a=0;
	string ext(actorFile.substr(actorFile.length()-3,3));
	
	if(ext==_T("act") || ext==_T("ACT"))
		loader=new ActorACTLoader;

	if(loader)
	{
		a=loader->loadActor(actorFile,mat,device);
		delete loader;
	}

	return a;
}