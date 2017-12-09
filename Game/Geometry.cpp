
#include "stdafx.h"
#include "Geometry.h"

extern ofstream			g_MediaOut;

void Geometry::computeBBox()
{
	_bBox=BBox3::InitBox;
	for(int v=0;v<_vxCount;++v)
		_bBox << _vertices[v].pos;
}

void Geometry::computeNormals()
{
	Vector3 n;
	unsigned short *t=_indices;
	for(int i=0;i<_triCount;++i)
	{
		n=(_vertices[t[1]].pos-_vertices[t[0]].pos) ^ (_vertices[t[2]].pos - _vertices[t[0]].pos);
		n.normalize();

		for(int v=0;v<3;++v)
			_vertices[t[v]].norm=n;

		t+=3;
	}
}

void Geometry::computeBinormalAndTangent()
{
	Vector3 sdir,tdir;
	Vector3 vDeltaPos1,vDeltaPos2;
	Vector2 vDeltaUV1,vDeltaUV2;

	const unsigned short *tri=_indices;
	for(int i=0;i<_triCount;++i,tri+=3)
	{
		Vx3D& v0=_vertices[tri[0]];
		Vx3D& v1=_vertices[tri[1]];
		Vx3D& v2=_vertices[tri[2]];

		vDeltaPos1=v1.pos - v0.pos;
		vDeltaPos2=v2.pos - v0.pos;

		vDeltaUV1=v1.uv - v0.uv;
		vDeltaUV2=v2.uv - v0.uv;

		float ir=(vDeltaUV1.x * vDeltaUV2.y - vDeltaUV1.y * vDeltaUV2.x);
		float r=ir==0.0f ? 0.0f : 1.0f / ir;

		sdir.x=(vDeltaUV2.y * vDeltaPos1.x) - (vDeltaUV1.y * vDeltaPos2.x);
		sdir.y=(vDeltaUV2.y * vDeltaPos1.y) - (vDeltaUV1.y * vDeltaPos2.y);
		sdir.z=(vDeltaUV2.y * vDeltaPos1.z) - (vDeltaUV1.y * vDeltaPos2.z);

		tdir.x=(vDeltaUV2.x * vDeltaPos1.x) - (vDeltaUV1.x * vDeltaPos2.x);
		tdir.y=(vDeltaUV2.x * vDeltaPos1.y) - (vDeltaUV1.x * vDeltaPos2.y);
		tdir.z=(vDeltaUV2.x * vDeltaPos1.z) - (vDeltaUV1.x * vDeltaPos2.z);

		sdir*=r;
		tdir*=r;
				
		v0.tangent=(sdir - v0.norm*(v0.norm*sdir));
		v1.tangent=(sdir - v1.norm*(v1.norm*sdir));
		v2.tangent=(sdir - v2.norm*(v2.norm*sdir));

		v0.tangent.normalize();
		v1.tangent.normalize();
		v2.tangent.normalize();

		v0.binorm=v0.tangent ^ v0.norm;
		if((v0.binorm * tdir) > 0.0f)
			v0.binorm*=-1.0f;
		v0.binorm.normalize();

		v1.binorm=v1.tangent ^ v1.norm;
		if((v1.binorm * tdir) > 0.0f)
			v1.binorm*=-1.0f;
		v1.binorm.normalize();
		
		v2.binorm=v2.tangent ^ v2.norm;
		if((v2.binorm * tdir) > 0.0f)
			v2.binorm*=-1.0f;
		v2.binorm.normalize();
	}
}

void Geometry::outputVertices() const
{
	g_MediaOut << "Vertices:" << endl;
	for(int i=0;i<_vxCount;++i)
		g_MediaOut << "- " << _vertices[i].pos << "; " << _vertices[i].norm << "; " << _vertices[i].uv << endl;
}

void Geometry::outputTriangles() const
{
	g_MediaOut << "Triangles:" << endl;
	for(int i=0;i<_triCount;++i)
		g_MediaOut << "- " << _indices[i*3] << "; " << _indices[i*3+1] << "; " << _indices[i*3+2] << endl;
}