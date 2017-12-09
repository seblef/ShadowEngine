
#include "stdafx.h"
#include "GeoBuilder.h"

namespace Core
{

void GeoBuilder::processMesh(IGameNode* n, IGameMesh* m, bool localSpace)
{
	_currentOffset=getVerticesOffset();
	Matrix4 moffset, moffsetNoRot, invObj;
	
	if(localSpace)
	{
		offsetMatrix(n,moffset,moffsetNoRot);

		GMatrix mIObject(n->GetObjectTM().Inverse());
		copyMatrix(mIObject,invObj);
		invObj(3,0)=0.0f;
		invObj(3,1)=0.0f;
		invObj(3,2)=0.0f;
		validMatrix(invObj);
	}
	else
	{
		moffset.createIdentity();
		moffsetNoRot.createIdentity();
	}

	processVertices(m,moffset,localSpace);
	processNormals(m,moffsetNoRot,localSpace);
	processUV(m);
	processTriangles(m);

	delete[] _verts;
	if(_norms)			delete[] _norms;
	if(_uv)				delete[] _uv;

	_verts=_norms=0;
	_uv=0;
}

void GeoBuilder::processVertices(IGameMesh* m, const Matrix4& trans, bool localSpace)
{
	_vCount=m->GetNumberOfVerts();
	_verts=new Vector3[_vCount];

	Vector3 vworld;

	for(int i=0;i<_vCount;++i)
	{
		copyP3(m->GetVertex(i,localSpace),_verts[i]);
		_verts[i]*=trans;

		copyP3(m->GetVertex(i,false),vworld);
		_box << vworld;
	}
}

void GeoBuilder::processNormals(IGameMesh* m, const Matrix4& trans, bool localSpace)
{
	_nCount=m->GetNumberOfNormals();
	if(_nCount > 0)
	{
		_norms=new Vector3[_nCount];
		for(int i=0;i<_nCount;++i)
		{
			copyP3(m->GetNormal(i,localSpace),_norms[i]);
			_norms[i]*=trans;
		}
	}
}

void GeoBuilder::processUV(IGameMesh* m)
{
	_tCount=m->GetNumberOfTexVerts();
	if(_tCount > 0)
	{
		_uv=new Vector2[_tCount];
		for(int i=0;i<_tCount;++i)
			copyP2(m->GetTexVertex(i),_uv[i]);
	}
}

};