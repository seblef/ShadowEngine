
#ifndef _IFILE_H_
#define _IFILE_H_

#include <string>

using namespace std;

namespace Core
{

enum FileAccess
{
	FA_READ,
	FA_WRITE,
	FA_UNKNOWN
};

enum FileSeek
{
	FS_START,
	FS_END,
	FS_CURRENT
};

class IFile
{
	protected:

		string					_fileName;
		FileAccess				_access;
		
	public:

		IFile(const string& fileName, FileAccess fa) : _fileName(fileName), _access(fa) {}
		virtual ~IFile() {}

		const string&			getFileName() const					{ return _fileName; }
		FileAccess				getAccess() const					{ return _access; }

		virtual bool			read(void *data, size_t size)=0;
		virtual bool			write(const void* data, size_t size)=0;

		virtual bool			good()=0;

		virtual bool			seek(size_t offset, FileSeek fs)=0;
		virtual size_t			tell()=0;
};

}

#endif