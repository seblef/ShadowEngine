
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Renderable.h"

struct LightCreate_t
{
	Color		_color;
	float		_range;
	bool		_castShadows;
	int			_shadowMapSize;
	int			_shadowMapFilter;
	float		_farAngle;
	float		_nearAngle;
	float		_areaWidth;
	float		_areaHeight;
	float		_nearZ;
	Vector3		_direction;
	Matrix4		_world;
};

class Light : public Renderable
{
public:

	enum LightType
	{
		LT_OMNI=0,
		LT_SPOT,
		LT_AREA,

		LT_COUNT
	};

protected:

	LightType				_type;
	Color					_color;
	float					_range;
	bool					_castShadows;
	int						_shadowMapSize;
	int						_shadowMapFilter;
	Vector3					_direction;

	IShadowMap*				_shadowMap;
	Matrix4					_shadowViewProj;

	void					updateWorldMatrix()
	{
		Matrix4 w(_worldMatrix);
		computeMatrix(_direction,w);
		setWorldMatrix(w);
	}

public:

	static void				computeMatrix(Vector3& d, Matrix4& m)
	{
		Vector3 pos(m);
		d.normalize();

		Quaternion qRot;
		qRot.buildRotationArc(d,Vector3::ZAxisVector);
		qRot.getMatrix(m);
		m=pos;
	}

	Light(LightType type, const LightCreate_t& c) : 
		Renderable(BBox3(-c._range,-c._range,-c._range,c._range,c._range,c._range),c._world,false),
		_type(type), _color(c._color), _range(c._range), _castShadows(c._castShadows), _direction(c._direction), _shadowMapSize(c._shadowMapSize),
		_shadowMapFilter(c._shadowMapFilter),	_shadowMap(0)	{}

	Light(const Light *l) : Renderable(l), _type(l->_type), _color(l->_color), _range(l->_range),
		_castShadows(l->_castShadows), _direction(l->_direction), _shadowMapSize(l->_shadowMapSize),
		_shadowMapFilter(l->_shadowMapFilter), _shadowMap(0)	{}
	virtual ~Light()																						{}

	LightType				getLightType() const						{ return _type; }
	float					getRange() const							{ return _range; }

	const Color&			getColor() const							{ return _color; }
	bool					getCastShadows() const						{ return _castShadows; }
	int						getShadowMapSize() const					{ return _shadowMapSize; }
	int						getShadowMapFilter() const					{ return _shadowMapFilter; }
	const Vector3&			getDirection() const						{ return _direction; }

	void					setRange(float r)							{ _range=r; _localBBox.setMin(Vector3(-r,-r,-r)); _localBBox.setMax(Vector3(r,r,r)); updateWorldMatrix(); }
	void					setColor(const Color& c)					{ _color=c; }
	void					setCastShadows(bool castShadows)			{ _castShadows=castShadows; }
	void					setShadowMapSize(int size)					{ _shadowMapSize=size; }
	void					setShadowMapFilter(int f)					{ _shadowMapFilter=f; }
	void					setDirection(const Vector3& dir)			{ _direction=dir; updateWorldMatrix(); }

	void					enqueue();

	void					setShadowMap(IShadowMap* m)					{ _shadowMap=m; }
	void					setShadowViewProjMatrix(const Matrix4& vp)	{ _shadowViewProj=vp; }

	IShadowMap*				getShadowMap() const						{ return _shadowMap; }
	const Matrix4&			getShadowViewProjMatrix() const				{ return _shadowViewProj; }
};

#endif