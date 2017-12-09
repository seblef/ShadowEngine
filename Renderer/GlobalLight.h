
#ifndef _GLOBALLIGHT_H_
#define _GLOBALLIGHT_H_

#include "ShadowMapBuffer.h"

class LightGlobal
{
protected:

	bool					_enable;

	IVideoDevice*			_device;
	IShader*				_shader;
	IConstantBuffer*		_CBuffer;
	ShadowMapBuffer*		_shadowBuffer;
	DepthStencilState		_DState;
	BlendState				_blendState;

	struct GlobalBuffer_t
	{
		Color				_color;
		Vector4				_dir;
		Color				_ambient;
	};

	GlobalBuffer_t			_lBuffer;

	IShadowMap*				_shadowMap;
	Matrix4					_shadowViewProj;

public:

	LightGlobal(IVideoDevice* device, ShadowMapBuffer* shadowBuffer);
	~LightGlobal();

	void					enableGlobalLight(const Color& color, const Vector3& dir);
	void					disableGlobalLight();
	bool					isGlobalLightEnable() const			{ return _enable; }

	void					applyLight() const;

	const Vector4&			getDirection() const				{ return _lBuffer._dir; }
	const Color&			getColor() const					{ return _lBuffer._color; }
	const Color&			getAmbientLight() const				{ return _lBuffer._ambient; }

	void					setDirection(const Vector4& d)		{ _lBuffer._dir=d; }
	void					setColor(const Color& c)			{ _lBuffer._color=c; }
	void					setAmbientLight(const Color& a)		{ _lBuffer._ambient=a; }

	IShadowMap*				getShadowMap() const				{ return _shadowMap; }
	void					setShadowMap(IShadowMap* m)			{ _shadowMap=m; }
	void					setShadowViewProjMatrix(const Matrix4& vp)	{ _shadowViewProj=vp; }
};

#endif