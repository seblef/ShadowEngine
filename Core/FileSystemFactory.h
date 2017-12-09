
#ifndef _FILESYSTEMFACTORY_H_
#define _FILESYSTEMFACTORY_H_

#include "IFileSystem.h"

namespace Core
{

class FileSystemFactory
{
protected:

	static IFileSystem*			_fileSys;

public:

	static void					createFileSystem(const string& fileSysName);
	static IFileSystem*			getFileSystem()									{ return _fileSys; }
};

}

#define g_FileSystem			Core::FileSystemFactory::getFileSystem()

#endif