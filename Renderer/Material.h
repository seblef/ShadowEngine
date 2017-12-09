
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <MediaCommon.h>
#include "MaterialStdBuffer.h"

#define MF_NONE						0
#define MF_ALPHA					(1 << 0)
#define MF_ADDPASS					(1 << 1)
#define MF_CASTNOSHADOWS			(1 << 2)

#define MF_ALL						(MF_ALPHA | MF_ADDPASS | MF_CASTNOSHADOWS)


class MaterialAnimation;

class Material
{
public:

	enum TextureSlot
	{
		TS_DIFFUSE=0,
		TS_NORMAL,
		TS_SPECULAR,
		TS_EMISSIVE,
		TS_ENVIONMENT,

		TS_COUNT
	};

protected:

	TVector<MaterialAnimation>	_anims;

	IVideoDevice*				_device;
	MaterialStdBuffer			_buffer;
	ITexture*					_textures[TS_COUNT];
	bool						_defaultTex[TS_COUNT];
	RenderState					_renderState;
	BlendState					_blendState;
	unsigned int				_flags;

	BlendMode					getBlendMode(const string& b) const;
	TextureSlot					getTextureSlot(const string& s) const;
	CullMode					getCullMode(const string& c) const;
	Vector2						parseVector2(ScriptFile& fl) const;

	virtual bool				parseToken(const string& token, ScriptFile& file);

public:

	Material(IVideoDevice *d);
	Material(ScriptFile& file, IVideoDevice *device);
	virtual ~Material();

	unsigned int				getFlag(unsigned int f) const				{ return _flags & f; }
	void						setFlag(unsigned int f)						{ _flags|=f; }
	void						unsetFlag(unsigned int f)					{ _flags&= ~f; }

	virtual void				setBase(float time);
	virtual void				setAdd(float time);
	virtual void				setTime(float time);

	MaterialStdBuffer&			getStdBuffer()								{ return _buffer; }
	const MaterialStdBuffer&	getStdBuffer() const						{ return _buffer; }

	bool						isAnimated() const							{ return !_anims.getVector().empty(); }
	int							getAnimationCount() const					{ return _anims.getVector().size(); }
	MaterialAnimation*			getAnimation(int n)	const					{ return _anims.getVector()[n]; }
	void						createAnimation(const string& animName);
	void						addAnimation(MaterialAnimation* a)			{ _anims.getVector().push_back(a); }
	void						deleteAnimation(int n);

	bool						isDefaultTexture(TextureSlot s) const		{ return _defaultTex[s]; }
	ITexture*					getTexture(TextureSlot s) const				{ return _textures[s] ;}
	void						setTexture(TextureSlot s, const string& texFile);

	BlendState					getBlendState() const						{ return _blendState; }
	RenderState					getRenderState() const						{ return _renderState; }

	void						setBlendState(BlendState bs)				{ _blendState=bs; }
	void						setRenderState(RenderState rs)				{ _renderState=rs; }

	void						updateStdBuffer();
};

const char * const g_MaterialTexNames[Material::TS_COUNT]={
	_T("diffuse"),
	_T("normal"),
	_T("specular"),
	_T("emissive"),
	_T("environment")
};

const char * const g_MaterialBlendNames[]=
{
	_T("zero"),
	_T("one"),
	_T("src_color"),
	_T("inv_src_color"),
	_T("src_alpha"),
	_T("inv_src_alpha"),
	_T("dest_alpha"),
	_T("inv_dest_alpha"),
	_T("dest_color"),
	_T("inv_dest_color"),
	_T("src_alpha_sat"),
	_T("both_src_alpha"),
	_T("both_inv_src_alpha"),
	_T("blend_factor"),
	_T("inv_blend_factor"),
	_T("src_color2"),
	_T("inv_src_color2")
};

#endif