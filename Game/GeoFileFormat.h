
#ifndef _GEOFILEFORMAT_H_
#define _GEOFILEFORMAT_H_

namespace File
{

#define GEO_HEADER			(('S' << 24) | ('G' << 16) | ('E' << 8) | ('O'))

struct GeoFileHeader
{
	unsigned int			_header;
	int						_vxCount;
	int						_triCount;
};

};

#endif