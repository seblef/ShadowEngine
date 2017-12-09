
#include "stdafx.h"
#include "GeoGEOLoader.h"
#include "GeoFileFormat.h"

Geometry* GeoGEOLoader::loadGeometry(const string& geoFile)
{
	Geometry* geo=0;

	string fileName(_T("Geometry/"));
	fileName+=geoFile;

	IFile *fl=FileSystemFactory::getFileSystem()->openFile(fileName,FA_READ);
	if(fl->good())
	{
		File::GeoFileHeader h;
		fl->read(&h,sizeof(File::GeoFileHeader));

		if(h._header==GEO_HEADER)
		{
			geo=new Geometry(h._vxCount,h._triCount);
			fl->read(geo->getVertices(),sizeof(Vx3D)*h._vxCount);
			fl->read(geo->getTriangles(),sizeof(unsigned short)*3*h._triCount);
		}
		else
			geo=new Geometry(3,1);
	}
	else
		geo=new Geometry(3,1);

	delete fl;

	geo->computeBBox();
	geo->computeBinormalAndTangent();
	return geo;
}