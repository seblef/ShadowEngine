
#include "StdFileList.h"
#include <stdlib.h>
#include <io.h>
#include <direct.h>

using namespace Core;

bool StdFileList::isGood(const string& file, const StringVector& filters) const
{
	if(filters.empty())
		return true;
	else
	{
		for(int i=0;i<filters.size();++i)
		{
			const string& ext(filters[i]);
			if(file.substr(file.length() - ext.length(),ext.length())==ext)
				return true;
		}

		return false;
	}
}

bool StdFileList::isGood(const string& file, const char** filters, int filterCount) const
{
	for (int i = 0; i<filterCount; ++i)
	{
		int extLength = strlen(filters[i]);
		if (file.substr(file.length() - extLength, extLength) == filters[i])
			return true;
	}

	return false;
}

void StdFileList::reset(const StringVector& filters, bool noDirs)
{
	_files.clear();
	_dirs.clear();

	char tmp[_MAX_PATH];
	_getcwd(tmp,_MAX_PATH);

	struct _finddata_t c_file;
	long hFile;

	string file;

	if((hFile=_findfirst("*",&c_file))!=-1L)
	{
		do
		{
			if(c_file.attrib & _A_SUBDIR)
			{
				if(!noDirs && strcmp(c_file.name,"..")!=0 && strcmp(c_file.name,".")!=0)
					_dirs.push_back(string(c_file.name));
			}
			else
			{
				file = c_file.name;
				if (isGood(file, filters))
					_files.push_back(file);
			}

		} while(_findnext(hFile,&c_file)==0);

		_findclose(hFile);
	}
}

void StdFileList::reset(const char** filters, int filterCount, bool noDirs)
{
	_files.clear();
	_dirs.clear();

	char tmp[_MAX_PATH];
	_getcwd(tmp, _MAX_PATH);

	struct _finddata_t c_file;
	long hFile;

	string file;

	if ((hFile = _findfirst("*", &c_file)) != -1L)
	{
		do
		{
			if (c_file.attrib & _A_SUBDIR)
			{
				if (!noDirs && strcmp(c_file.name, "..") != 0 && strcmp(c_file.name, ".") != 0)
					_dirs.push_back(string(c_file.name));
			}
			else
			{
				file = c_file.name;
				if (isGood(file, filters, filterCount))
					_files.push_back(file);
			}

		} while (_findnext(hFile, &c_file) == 0);

		_findclose(hFile);
	}
}

