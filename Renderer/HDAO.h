
#ifndef _HDAO_H_
#define _HDAO_H_

#include <MediaCommon.h>
#include "GBuffer.h"

class HDAO : public TSingleton<HDAO>
{
protected:

	bool				_enable;

	struct HDAOBufffer_t
	{
		Vector2			_renderTargetSize;
		float			_acceptAngle;
		float			_q;
		float			_qTimesNear;
		float			_normalScale;
		float			_acceptRadius;
		float			_rejectRadius;
		float			_intensity;
		Vector3			_pack;
	};

	HDAOBufffer_t		_hBuffer;

	IVideoDevice*		_device;
	IShader*			_shader;
	ITexture*			_HDAOTexture;
	IConstantBuffer*	_HDAOBuffer;
	BlendState			_blendState;
	SamplerState		_samplerState;

public:

	HDAO(IVideoDevice *d, const Config& cfg);
	~HDAO();

	void				process(GBuffer& gbuf, Camera& cam);
	void				onResize(int w, int h);

	ITexture*			getHDAOTexture() const					{ return _HDAOTexture; }

	float&				getAcceptAngle()						{ return _hBuffer._acceptAngle; }
	float&				getNormalScale()						{ return _hBuffer._normalScale; }
	float&				getAcceptRadius()						{ return _hBuffer._acceptRadius; }
	float&				getRejectRadius()						{ return _hBuffer._rejectRadius; }
	float&				getIntensity()							{ return _hBuffer._intensity; }
};

#endif