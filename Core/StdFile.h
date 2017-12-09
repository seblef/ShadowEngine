
#ifndef _STDFILE_H_
#define _STDFILE_H_

#include "IFile.h"
#include <fstream>
#include <assert.h>

namespace Core
{

class StdFile : public IFile
{
protected:

	fstream			_fs;

public:

	StdFile(const string& fileName, FileAccess fa) : IFile(fileName,fa), _fs(fileName,(fa==FA_WRITE ? ios::out : ios::in) | ios::binary)		{}
	~StdFile()					{}

	bool			read(void *data, size_t size)				{ assert(_access==FA_READ);		_fs.read((char*)data,size);		return _fs.good(); }
	bool			write(const void* data, size_t size)		{ assert(_access==FA_WRITE);	_fs.write((const char*)data,size);	return _fs.good(); }

	bool			good()										{ return _fs.good(); }

	bool			seek(size_t offset, FileSeek fs);
	size_t			tell()										{ return _fs.tellp(); }
};

}

#endif