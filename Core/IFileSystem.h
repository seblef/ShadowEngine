
#ifndef _IFILESYSTEM_H_
#define _IFILESYSTEM_H_

#include "IFile.h"
#include "IFileList.h"

namespace Core
{

class IFileSystem
{
	protected:

		string						_workingDir;
		string						_startDir;

	public:

		IFileSystem() {}
		virtual ~IFileSystem() {}

		virtual bool				changeWorkingDirectory(const string& dir)=0;
		virtual void				resetWorkingDirectory()=0;
		virtual bool				directoryExists(const string& dir)=0;

		const string&				getWorkingDirectory() const					{ return _workingDir; }
		const string&				getStartDirectory() const					{ return _startDir; }

		virtual bool				createDirectory(const string& dir)=0;

		bool						hasPath(const string& fileName);
		bool						extractPath(const string& fileName, string& path);
		bool						extractFileName(const string& fullFileName, string& fileName);
		bool						extractLocalFileName(const string& fileName, string& localFileName);

		virtual IFileList*			createFileList(const StringVector& filters, bool noDirs)=0;
		virtual IFileList*			createFileList(const char** filters, int filterCount, bool noDirs) = 0;

		virtual IFile*				openFile(const string& fileName, FileAccess fa) = 0;
};

}

#endif