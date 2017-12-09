
#include "stdafx.h"
#include "GeometryLoader.h"
#include "Geo3DSLoader.h"
#include "GeoGEOLoader.h"

extern ofstream				g_MediaOut;

Geometry* GeometryLoader::loadGeometry(const string& file)
{
	IGeometryLoader* loader=0;
	Geometry *geo=0;
	string ext(file.substr(file.length()-3,3));

	if(ext==_T("3ds") || ext==_T("3DS"))
		loader=new Geo3DSLoader;
	else if(ext==_T("geo") || ext==_T("GEO"))
		loader=new GeoGEOLoader;

	if(loader)
	{
		geo=loader->loadGeometry(file);
		delete loader;
	}
	else
		g_MediaOut << "x [GeometryLoader::loadGeometry]: Unknown file format (" << file << ")" << endl;

	return geo;
}