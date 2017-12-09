
#include "StdFile.h"

using namespace Core;

bool StdFile::seek(size_t offset, FileSeek fs)
{
	ios::seekdir sd;
	switch(fs)
	{
	case FS_START:			sd=ios::beg;		break;
	case FS_END:			sd=ios::end;		break;
	default:				sd=ios::cur;
	}

	_fs.seekp(offset,sd);
	return _fs.good();
}
