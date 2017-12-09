
#include "IFileSystem.h"

using namespace Core;

bool IFileSystem::hasPath(const string& fileName)
{
	bool path=false;
	for(int c=0;c<fileName.length();++c)
		path|= (fileName[c]=='\\' || fileName[c]=='/');

	return path;
}

bool IFileSystem::extractPath(const string& fileName, string& path)
{
	int c=fileName.length()-1;
	while(c >= 0 && fileName[c]!='\\' && fileName[c]!='/')
		--c;

	if(c > 0)
	{
		path=fileName.substr(0,c);
		return true;
	}
	else
		return false;
}

bool IFileSystem::extractFileName(const string& fullFileName, string& fileName)
{
	int c=fullFileName.length()-1;
	while(c >= 0 && fullFileName[c]!='\\' && fullFileName[c]!='/')
		--c;

	if(c > 0)
	{
		fileName=fullFileName.substr(c+1,fullFileName.length() - (c+1));
		return true;
	}
	else
		return false;
}

bool IFileSystem::extractLocalFileName(const string& fileName, string& shadowFileName)
{
	if(fileName.find(_startDir)!=string::npos)
	{
		shadowFileName=fileName.substr(_startDir.length()+1,fileName.length() - _startDir.length());
		return true;
	}
	else
	{
		shadowFileName=fileName;
		return false;
	}
}
