
#ifndef _IFILELIST_H_
#define _IFILELIST_H_

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

namespace Core
{

typedef vector<string>			StringVector;

class IFileList
{
	protected:

		StringVector			_files;
		StringVector			_dirs;

	public:

		IFileList() {}
		virtual ~IFileList() {}

		int						getFileCount() const				{ return _files.size(); }
		int						getDirCount() const					{ return _dirs.size(); }

		const string&			getFile(int n) const				{ return _files[n]; }
		const string&			getDir(int n) const					{ return _dirs[n]; }

		void					sortFiles()
		{
			sort(_files.begin(), _files.end());
		}
		void					sortDirs()
		{
			sort(_dirs.begin(), _dirs.end());
		}

		virtual void			reset(const StringVector& filter, bool noDirs=false)=0;
		virtual void			reset(const char** filters, int filterCount, bool noDirs = false) = 0;
};

}

#endif