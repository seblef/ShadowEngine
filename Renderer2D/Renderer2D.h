
#ifndef _RENDERER2D_H_
#define _RENDERER2D_H_

#include "R2D_Frame.h"
#include "R2D_Renderer.h"
#include "R2D_Font.h"

class Renderer2D : public TSingleton<Renderer2D>
{
protected:

	IVideoDevice*					_device;
	IShader*						_shader;
	ITexture*						_whiteTexture;
	BlendState						_blendState;
	RenderState						_renderState;
	DepthStencilState				_depthState;
	SamplerState					_samplerState;

	R2D_Renderer*					_renderers[R2D_Object::R2D_COUNT];

	TDataBase<R2D_Font>				_fontDB;
	TDataBase<ITexture>				_textureDB;
	TDataBase<R2D_Frame>			_frameDB;

	R2D_Frame*						_currentFrame;

	void							renderLayer(const R2D_Layer& l) const;

public:

	Renderer2D(IVideoDevice* device);
	~Renderer2D();

	IVideoDevice*					getDevice() const									{ return _device; }

	R2D_Frame*						loadFrame(const string& frameFile);

	TDataBase<R2D_Font>&			getFontDB()											{ return _fontDB; }
	TDataBase<ITexture>&			getTextureDB()										{ return _textureDB; }
	TDataBase<R2D_Frame>&			getFrameDB()										{ return _frameDB; }

	ITexture*						getWhiteTexture() const								{ return _whiteTexture; }

	void							setCurrentFrame(R2D_Frame* frame)					{ _currentFrame=frame; }
	void							setCurrentFrame(const string& frame)				{ setCurrentFrame(_frameDB.getData(frame)); }
	R2D_Frame*						getCurrentFrame() const								{ return _currentFrame; }

	void							render();
};

#endif