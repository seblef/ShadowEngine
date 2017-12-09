
#ifndef _TGEOBUILDER_H_
#define _TGEOBUILDER_H_

#include "GeoBuilder.h"

namespace Core
{

template<typename T>
class TGeoBuilder : public GeoBuilder
{
protected:

	TDynamicTab<T>				_vertices;

	int							getVerticesOffset()						{ return _vertices.getCount(); }

	void						processTriangles(IGameMesh* m);
	void						computeBinormalAndTangent(T *v) const;
	unsigned short				addVertex(const T& v);
	bool						areVerticesEquals(const T& v1, const T& v2) const
	{
		return v1.pos==v2.pos && v1.norm==v2.norm && v1.uv==v2.uv;
	}

public:

	TGeoBuilder()			{}
	~TGeoBuilder()			{}

	int								getVerticesCount() const		{ return _vertices.getCount(); }
	const void*						getVerticesBuffer() const		{ return (const void*)_vertices.getBuffer(); }
	size_t							getVertexSize() const			{ return sizeof(T); }
	TDynamicTab<T>&					getVertices()					{ return _vertices; }
};

template<typename T>
void TGeoBuilder<T>::processTriangles(IGameMesh* m)
{
	FaceEx *f;
	T v[3];
	for(int i=0;i<m->GetNumberOfFaces();++i)
	{
		f=m->GetFace(i);
		for(int j=0;j<3;++j)
		{
			v[j].pos=_verts[f->vert[j]];
			v[j].norm=_norms[f->norm[j]];
			v[j].uv=_uv[f->texCoord[j]];
		}

		computeBinormalAndTangent(v);

		for(int j=0;j<3;++j)
			_indices.add(addVertex(v[j]));
	}
}

template<typename T>
void TGeoBuilder<T>::computeBinormalAndTangent(T* v) const
{
	Vector3 sdir,tdir;
	Vector3 vDeltaPos1,vDeltaPos2;
	Vector2 vDeltaUV1,vDeltaUV2;

	vDeltaPos1=v[1].pos - v[0].pos;
	vDeltaPos2=v[2].pos - v[0].pos;

	vDeltaUV1=v[1].uv - v[0].uv;
	vDeltaUV2=v[2].uv - v[0].uv;

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

	for(int i=0;i<3;++i)
	{
		v[i].tangent=(sdir - v[i].norm*(v[i].norm*sdir));
		v[i].tangent.normalize();

		v[i].binorm=v[i].tangent ^ v[i].norm;
		if((v[i].binorm * tdir) > 0)
			v[i].binorm.negativeThis();

		v[i].binorm.normalize();
	}
}

template<typename T>
unsigned short TGeoBuilder<T>::addVertex(const T& v)
{
	int r=-1;
	for(int i=_currentOffset;i<_vertices.getCount();++i)
	{
		if(areVerticesEquals(v,_vertices[i]))
			r=i;
	}

	if(r==-1)
	{
		r=_vertices.getCount();
		_vertices.add(v);
	}
	return (unsigned short)r;
}

};

#endif