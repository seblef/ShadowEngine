
#ifndef _MATERIALSTDBUFFER_H_
#define _MATERIALSTDBUFFER_H_

#include <Core.h>

class MaterialStdBuffer
{
protected:

	Color			_ambient;
	Color			_diffuse;
	Color			_specular;
	Vector4			_shininess;		// x=Shininess y=Intensity

	Vector2			_UVOffset;
	Vector2			_UVScale;
	Matrix2			_UVRot;

public:

	MaterialStdBuffer() : _ambient(Color::Black), _diffuse(Color::White),
		_specular(Color::Black), _UVOffset(Vector2::NullVector), _UVScale(Vector2::UnitVector),
		_UVRot(Matrix2::Identity), _shininess(Vector4::NullVector)									{}
	
	MaterialStdBuffer(const MaterialStdBuffer& m) : _ambient(m._ambient), _diffuse(m._diffuse), _specular(m._specular),
		_UVOffset(m._UVOffset), _UVScale(m._UVScale), _UVRot(m._UVRot), _shininess(m._shininess)			{}

	void			setAmbient(const Color& c)						{ _ambient=c; }
	void			setDiffuse(const Color& c)						{ _diffuse=c; }
	void			setSpecular(const Color& c)						{ _specular=c; }
	void			setShininess(float s)							{ _shininess.x=s; }
	void			setSpecIntensity(float i)						{ _shininess.y=i; }
	
	void			setOffset(const Vector2& o)						{ _UVOffset=o; }
	void			setScale(const Vector2& s)						{ _UVScale=s; }
	void			setRotation(const Matrix2& r)					{ _UVRot=r; }

	const Color&	getAmbient() const								{ return _ambient; }
	const Color&	getDiffuse() const								{ return _diffuse; }
	const Color&	getSpecular() const								{ return _specular; }
	float			getShininess() const							{ return _shininess.x; }
	float			getSpecIntensity() const						{ return _shininess.y; }

	const Vector2&	getOffset() const								{ return _UVOffset; }
	const Vector2&	getScale() const								{ return _UVScale; }
	const Matrix2&	getRotation() const								{ return _UVRot; }

	MaterialStdBuffer&	operator=(const MaterialStdBuffer& m)
	{
		_ambient=m._ambient;		_diffuse=m._diffuse;		_specular=m._specular;	_shininess=m._shininess;
		_UVOffset=m._UVOffset;		_UVScale=m._UVScale;		_UVRot=m._UVRot;
		return *this;
	}

	operator const float*()	const									{ return (const float*)this; }
};


#endif