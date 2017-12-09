
#ifndef _PARTICLEMATERIAL_H_
#define _PARTICLEMATERIAL_H_

#include "ParticleTextureSet.h"

class ParticleMaterial
{
protected:

	IVideoDevice*			_device;

	ITexture*				_texture;
	ParticleTextureSet*		_textureSet;

	BlendState				_blendState;

	BlendMode				getBlendMode(const string& bm) const;

public:

	ParticleMaterial(const ParticleMaterial& m);
	ParticleMaterial(ScriptFile& sf);
	ParticleMaterial(const string& texFile, bool fileIsDir,
		BlendMode srcBlend=BLEND_SRCALPHA,
		BlendMode destBlend=BLEND_INVSRCALPHA);
	ParticleMaterial(const vector<string>& texSet,
		BlendMode srcBlend=BLEND_SRCALPHA,
		BlendMode destBlend=BLEND_INVSRCALPHA);
	~ParticleMaterial();

	ITexture*				getTexture() const					{ return _texture; }
	ParticleTextureSet*		getTextureSet() const				{ return _textureSet; }

	ITexture*				getTexture(float percent) const;
	BlendState				getBlendState() const				{ return _blendState; }
};

#endif