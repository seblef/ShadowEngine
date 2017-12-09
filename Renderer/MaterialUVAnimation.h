
#ifndef _MATERIALUVANIMATION_H_
#define _MATERIALUVANIMATION_H_

#include "MaterialAnimation.h"

class MaterialUVAnimation : public MaterialAnimation
{
protected:

	Vector2				_offsetPerSec;
	float				_rotPerSec;

public:

	MaterialUVAnimation(Material *m) : MaterialAnimation(m),
		_offsetPerSec(Vector2::NullVector), _rotPerSec(0)					{}
	MaterialUVAnimation(Material *m, const Vector2& offset, float rot) :
		MaterialAnimation(m), _offsetPerSec(offset), _rotPerSec(rot)		{}
	MaterialUVAnimation(const MaterialUVAnimation& a) : MaterialAnimation(a._material),
		_offsetPerSec(a._offsetPerSec), _rotPerSec(a._rotPerSec)			{}
	MaterialUVAnimation(Material *m, ScriptFile& sf);

	const char*	getAnimationName() const				{ return "uv"; }

	void				set(float time)
	{
		_material->getStdBuffer().setOffset(_offsetPerSec * time);
		Matrix2 m;
		m.rotation(_rotPerSec * time * M_PI / 180);
		_material->getStdBuffer().setRotation(m);
	}

	const Vector2&		getOffset() const				{ return _offsetPerSec; }
	float				getRotation() const				{ return _rotPerSec; }

	void				setOffset(const Vector2& o)		{ _offsetPerSec=o; }
	void				setRotation(float r)			{ _rotPerSec=r; }
};

#endif