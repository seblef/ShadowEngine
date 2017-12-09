
#include "stdafx.h"
#include "Geo3DSLoader.h"

Geometry* Geo3DSLoader::loadGeometry(const string& geoFileName)
{
	string fileName(_T("Geometry/"));
	fileName+=geoFileName;

	file3ds *file;
	database3ds *db=0;
	Geometry* geo=0;

	if((file=OpenFile3ds(fileName.c_str(),"r"))==0)
		return new Geometry(3,1);

	InitDatabase3ds(&db);
	CreateDatabase3ds(file,db);
	DisconnectDatabase3ds(db);

	geo=loadMesh(db);

	ReleaseDatabase3ds(&db);
	CloseAllFiles3ds();

	return geo;
}

Geometry* Geo3DSLoader::loadMesh(database3ds* db)
{
	Geometry* geo=0;

	if(GetMeshCount3ds(db)==0)
		return new Geometry(3,1);

	mesh3ds *m3ds=0;
	InitMeshObj3ds(&m3ds,65535,65535, InitTextArray3ds | InitSmoothArray3ds | InitVFlagArray3ds);
	GetMeshByIndex3ds(db,0,&m3ds);

	geo=new Geometry(m3ds->nfaces*3,m3ds->nfaces);
	
	Vx3D *vx=geo->getVertices();
	unsigned short *tri=geo->getTriangles();
	unsigned short t[3];

	for(int i=0;i<m3ds->nfaces;++i)
	{
		t[0]=m3ds->facearray[i].v1;
		t[1]=m3ds->facearray[i].v3;
		t[2]=m3ds->facearray[i].v2;

		tri[0]=i*3;
		tri[1]=i*3+1;
		tri[2]=i*3+2;

		for(int v=0;v<3;++v)
		{
			vx[v].pos=Vector3(m3ds->vertexarray[t[v]].x,m3ds->vertexarray[t[v]].z,m3ds->vertexarray[t[v]].y);
			if(t[v] < m3ds->ntextverts)
				vx[v].uv=Vector2(m3ds->textarray[t[v]].u,m3ds->textarray[t[v]].v);
			else
				vx[v].uv=Vector2::NullVector;
		}

		vx+=3;
		tri+=3;
	}

	geo->computeBBox();
	geo->computeNormals();
	geo->computeBinormalAndTangent();

	return geo;
}