
#ifndef _STDFILELIST_H_
#define _STDFILELIST_H_

#include "IFileList.h"

namespace Core
{

class StdFileList : public IFileList
{
	protected:

		bool				isGood(const string& file, const StringVector& filters) const;
		bool				isGood(const string& file, const char** filters, int filterCount) const;

	public:

		StdFileList(const StringVector& filters, bool noDirs=false)					{ reset(filters,noDirs); }
		StdFileList(const char **filters, int filterCount, bool noDirs = false)		{ reset(filters, filterCount, noDirs); }
		~StdFileList()										{}

		void				reset(const StringVector& filters, bool noDirs=false);
		void				reset(const char** filters, int filterCount, bool noDirs = false);
};

}

#endif