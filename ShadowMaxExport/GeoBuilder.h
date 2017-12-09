
#ifndef _GEOBUILDER_H_
#define _GEOBUILDER_H_

#include <IGame/IGame.h>
#include <Core.h>
#include "SUtils.h"

namespace Core
{

class GeoBuilder
{
protected:

	BBox3						_box;

	int							_vCount;
	int							_nCount;
	int							_tCount;

	Vector3						*_verts;
	Vector3						*_norms;
	Vector2						*_uv;

	TDynamicTab<unsigned short>	_indices;

	int							_currentOffset;

	void						processVertices(IGameMesh* m, const Matrix4& trans, bool localSpace);
	void						processNormals(IGameMesh* m, const Matrix4& trans, bool localSpace);
	void						processUV(IGameMesh* m);

	virtual void				processTriangles(IGameMesh* m)=0;
	virtual int					getVerticesOffset()=0;

public:

	GeoBuilder() : _verts(0), _norms(0), _uv(0), _box(1e23f,1e23f,1e23f,-1e23f,-1e23f,-1e23f),
		_currentOffset(0)	{}
	virtual ~GeoBuilder()			{}

	const BBox3&					getBBox() const					{ return _box; }
	TDynamicTab<unsigned short>&	getIndices()					{ return _indices; }

	virtual int						getVerticesCount() const=0;
	virtual const void*				getVerticesBuffer() const=0;
	virtual size_t					getVertexSize() const=0;

	void							processMesh(IGameNode* n, IGameMesh* m, bool localSpace);
};



};

#endif