
#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Vector2.h"
#include "Vector4.h"

namespace Core
{

enum VertexType
{
	VX_3D=0,
	VX_3DSKIN,
	VX_2D,
	VX_BASE,

	VX_COUNT

};

class VxBase
{
public:

	Vector3			pos;
	Vector4			color;
	Vector2			uv;

	VxBase()		{}
	VxBase(const Vector3& _pos, const Vector4& _color, const Vector2& _uv) : pos(_pos), color(_color), uv(_uv)		{}
	VxBase(const VxBase& v) : pos(v.pos), color(v.color), uv(v.uv)			{}
	~VxBase()		{}

	VxBase&			operator=(const VxBase& v)			{ pos=v.pos; color=v.color; uv=v.uv; return *this; }
};

class Vx2D
{
public:

	Vector3			pos;
	float			w;
	Vector4			color;
	Vector2			uv;

	Vx2D() : w(1.0f)	{}
	Vx2D(const Vector3& _pos, const Vector4& _color, const Vector2& _uv) : pos(_pos), w(1.0f), color(_color), uv(_uv)	{}
	Vx2D(const Vx2D& v) : pos(v.pos), w(1.0f), color(v.color), uv(v.uv)	{}
	~Vx2D()				{}

	Vx2D&			operator=(const Vx2D& v)			{ pos=v.pos; color=v.color; uv=v.uv; return *this; }
};

class Vx3D
{
public:

	Vector3			pos;
	Vector3			norm;
	Vector3			tangent;
	Vector3			binorm;
	Vector2			uv;

	Vx3D()			{}
	Vx3D(const Vector3& _pos, const Vector3& _normal, const Vector2& _uv) : pos(_pos), norm(_normal), uv(_uv)	{}
	Vx3D(const Vector3& _pos, const Vector3& _norm, const Vector3& _tan, const Vector3& _binorm, const Vector2& _uv) :
		pos(_pos), norm(_norm), tangent(_tan), binorm(_binorm), uv(_uv)	{}
	Vx3D(const Vx3D& v) : pos(v.pos), norm(v.norm), tangent(v.tangent), binorm(v.binorm), uv(v.uv)		{}
	~Vx3D()			{}

	Vx3D&			operator=(const Vx3D& v)		{ pos=v.pos; norm=v.norm; tangent=v.tangent; binorm=v.binorm; uv=v.uv; return *this; }
};

class Vx3DSkin
{
public:

	Vector3			pos;
	Vector4			boneWeights;
	union
	{
		unsigned char	boneIndices[4];
		unsigned int	dwBoneIndices;
	};
	Vector3			norm;
	Vector3			tangent;
	Vector3			binorm;
	Vector2			uv;

	Vx3DSkin()		{}
	Vx3DSkin(const Vx3DSkin& v) : pos(v.pos), boneWeights(v.boneWeights), dwBoneIndices(v.dwBoneIndices), norm(v.norm), tangent(v.tangent), binorm(v.binorm), uv(v.uv)	{}
	~Vx3DSkin()		{}

	Vx3DSkin&		operator=(const Vx3DSkin& v)
	{
		pos=v.pos;
		boneWeights=v.boneWeights;
		dwBoneIndices=v.dwBoneIndices;
		norm=v.norm;
		tangent=v.tangent;
		binorm=v.binorm;
		uv=v.uv;
		return *this;
	}
};

const size_t g_VertexSize[VX_COUNT]={
	sizeof(Vx3D),
	sizeof(Vx3DSkin),
	sizeof(Vx2D),
	sizeof(VxBase)
};

inline ostream& operator << (ostream& os, Vx3D& v)
{
	os << "Vert: " << v.pos << "; " << v.norm << "; " << v.uv << endl;
	return os;
}

inline ostream& operator << (ostream& os, Vx3DSkin& v)
{
	os << "Vert: " << v.pos << "; " << v.norm << "; " << v.uv << "; (" << v.boneIndices[0] << "," << v.boneIndices[1] << ", " << v.boneIndices[2] << "); ("<<
		v.boneWeights[0] << ", " << v.boneWeights[1] << ", " << v.boneWeights[2] << ")" << endl;
	return os;
}

}


#endif