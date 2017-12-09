
#ifndef _GROUNDFILEFORMAT_H_
#define _GROUNDFILEFORMAT_H_

#define GROUND_HEADER			(('S' << 24) | ('G' << 16) | ('R' << 8) | ('D'))

namespace File
{

struct GroundFileHeader
{
	unsigned int			_header;
	int						_width;
	int						_height;
	int						_matCount;
	int						_gameMatCount;
};

struct GroundFileMaterial
{
	char					_name[64];
};

};

#endif