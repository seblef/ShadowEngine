
#ifndef _FONTFILEFORMAT_H_
#define _FONTFILEFORMAT_H_

namespace File
{

#define FONT_HEADER				(('S' << 24) | ('F' << 16) | ('N' << 8) | ('T'))

struct FontFileHeader
{
	unsigned int		_header;
	char				_texFile[64];
};

};

#endif