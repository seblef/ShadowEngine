
#include "stdafx.h"
#include "ActorAnimLoader.h"
#include "ActorFileFormat.h"

AnimPRSMultiTrack* ActorAnimLoader::loadActorAnimation(const string& animFile)
{
	AnimPRSMultiTrack* a=0;
	IFile *fl=FileSystemFactory::getFileSystem()->openFile(animFile,FA_READ);
	if(fl)
	{
		File::ActorAnimFileHeader h;
		fl->read(&h,sizeof(File::ActorAnimFileHeader));

		if(h._header==ACTORANIM_HEADER)
		{
			a=new AnimPRSMultiTrack(h._keyCount,h._trackCount);
			fl->read(a->getTimes(),sizeof(float)*h._keyCount);
			fl->read(a->getValues(),sizeof(AnimPRSKeyVal)*h._keyCount*h._trackCount);
		}

		delete fl;
	}

	return a;
}