
#ifndef _GROUNDTRI_H_
#define _GROUNDTRI_H_

#include <Core.h>

class GroundTri
{
protected:

	Vx3D		_verts[3];
	int			_matID;

public:

	GroundTri() : _matID(0)					{}
	GroundTri(int x, int y, int side, int mat_id)
	{
		create(x,y,side,mat_id);
	}
	GroundTri(const GroundTri& t) : _matID(t._matID)
	{
		for(int i=0;i<3;++i)
			_verts[i]=t._verts[i];
	}

	GroundTri&			operator=(const GroundTri& t)
	{
		_matID=t._matID;
		for(int i=0;i<3;++i)
			_verts[i]=t._verts[i];
		return *this;
	}

	bool				operator < (const GroundTri& t) const				{ return _matID < t._matID; }

	int					getMatID() const				{ return _matID; }
	void				setMatID(int m)					{ _matID=m; }

	const Vx3D&			getVertice(int n) const			{ return _verts[n]; }
	const Vx3D*			getVertices() const				{ return _verts; }

	void				create(int x, int y, int side, int mat_id)
	{
		_matID=mat_id;
		for(int i=0;i<3;++i)
		{
			_verts[i].norm=Vector3::YAxisVector;
			_verts[i].tangent=Vector3::XAxisVector;
			_verts[i].binorm=Vector3::ZAxisVector;
			_verts[i].pos.y=0.0f;
		}

		float fx=(float)x;
		float fy=(float)y;

		_verts[0].pos.x=_verts[0].uv.x=fx;
		_verts[0].pos.z=_verts[0].uv.y=fy;

		if(side==0)
		{
			_verts[2].pos.x=_verts[2].uv.x=fx+1;
			_verts[2].pos.z=_verts[2].uv.y=fy+1;
			_verts[1].pos.x=_verts[1].uv.x=fx;
			_verts[1].pos.z=_verts[1].uv.y=fy+1;
		}
		else
		{
			_verts[2].pos.x=_verts[2].uv.x=fx+1;
			_verts[2].pos.z=_verts[2].uv.y=fy;
			_verts[1].pos.x=_verts[1].uv.x=fx+1;
			_verts[1].pos.z=_verts[1].uv.y=fy+1;
		}
	}
};

#endif