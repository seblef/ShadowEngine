
#ifndef _STDFILESYSTEM_H_
#define _STDFILESYSTEM_H_

#include "IFileSystem.h"
#include "StdFile.h"
#include "StdFileList.h"
#include <direct.h>

namespace Core
{

class StdFileSystem : public IFileSystem
{
protected:

	void				getCurrentDir(string& dir)
	{
		char sz[2048];
		_getcwd(sz,2048);
		dir=sz;
	}

public:

	StdFileSystem()
	{
		getCurrentDir(_startDir);
		_workingDir=_startDir;
	}
	~StdFileSystem()				{}

	bool				changeWorkingDirectory(const string& dir)
	{
		bool ok=(_chdir(dir.c_str())==0);
		getCurrentDir(_workingDir);
		return ok;
	}
	void				resetWorkingDirectory()												{ changeWorkingDirectory(_startDir); }
	bool				directoryExists(const string& dir)
	{
		string temp(_workingDir);
		if(changeWorkingDirectory(dir))
		{
			changeWorkingDirectory(temp);
			return true;
		}
		else
			return false;
	}

	bool				createDirectory(const string& dir)									{ return _mkdir(dir.c_str())==0; }

	IFileList*			createFileList(const StringVector& filters, bool noDirs)			{ return new StdFileList(filters,noDirs); }
	IFileList*			createFileList(const char** filters, int filterCount, bool noDirs)	{ return new StdFileList(filters, filterCount, noDirs); }
	IFile*				openFile(const string& fileName, FileAccess fa)						{ return new StdFile(fileName,fa); }
};

}

#endif