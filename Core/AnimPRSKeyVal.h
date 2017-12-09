
#ifndef _ANIMPRSKEYVAL_H_
#define _ANIMPRSKEYVAL_H_

#include "Vector3.h"

namespace Core
{

class AnimPRSKeyVal
{
protected:

	Vector3				_pos;
	Matrix3				_rotScale;

public:

	AnimPRSKeyVal()		{}
	AnimPRSKeyVal(const Vector3& pos, const Matrix3& rotScale) : _pos(pos), _rotScale(rotScale)			{}
	AnimPRSKeyVal(const Matrix4& m) : _pos(m)
	{
		m.getMatrix3(_rotScale);
	}
	AnimPRSKeyVal(const AnimPRSKeyVal& k) : _pos(k._pos), _rotScale(k._rotScale)			{}

	AnimPRSKeyVal&		operator=(const AnimPRSKeyVal& k)
	{
		_pos=k._pos;
		_rotScale=k._rotScale;
		return *this;
	}

	AnimPRSKeyVal&		operator=(const Matrix4& m)
	{
		_pos=m;
		m.getMatrix3(_rotScale);
		return *this;
	}

	void				setPosition(const Vector3& pos)						{ _pos=pos; }
	void				setRotationScale(const Matrix3& rotScale)			{ _rotScale=rotScale; }

	const Vector3&		getPosition() const									{ return _pos; }
	const Matrix3&		getRotationScale() const							{ return _rotScale; }

	void				getMatrix4(Matrix4& m)
	{
		m=_rotScale;
		m=_pos;
	}
};

};

#endif