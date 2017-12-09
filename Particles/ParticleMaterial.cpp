
#include "stdafx.h"
#include "ParticleMaterial.h"
#include "ParticleTextureStore.h"
#include "ParticleEngine.h"

const char * const g_ParticleBlendNames[]=
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

ParticleMaterial::ParticleMaterial(const ParticleMaterial& m) : _texture(0), _textureSet(0), _device(m._device)
{
	if (m._textureSet)
		_textureSet = new ParticleTextureSet(*m._textureSet);
	else if (m._texture)
		_texture = ParticleTextureStore::getSingletonRef().getTexture(m._texture->getName());

	BlendMode src, dest;
	bool blend;

	ParticleEngine::getSingletonRef().getVideoDevice()->getBlendStateDesc(m._blendState, blend, src, dest);
	_blendState = ParticleEngine::getSingletonRef().getVideoDevice()->createBlendState(blend, src, dest);
}

ParticleMaterial::ParticleMaterial(ScriptFile& sf) :  _texture(0), _textureSet(0), _blendState(0)
{
	_device = ParticleEngine::getSingletonRef().getVideoDevice();

	string token(sf.getToken());
	while(sf.good() && token!="end_material")
	{
		if(token=="texture")
			_texture=ParticleTextureStore::getSingletonRef().getTexture(sf.getToken());
		else if (token == "texture_set")
			_textureSet = new ParticleTextureSet(sf);
		else if (token == "texture_dir")
			_textureSet = new ParticleTextureSet(sf.getToken());
		else if(token=="blend")
		{
			BlendMode src,dest;
			src=getBlendMode(sf.getToken());
			dest=getBlendMode(sf.getToken());
			_blendState = _device->createBlendState(true, src, dest);
		}

		token=sf.getToken();
	}

//	if((!_texture || !_texture->isGood()) && !_textureSet)
//		_texture=ParticleTextureStore::getSingletonRef().getTexture("Textures/Default/Diffuse.bmp");

	if(!_blendState)
		_blendState=_device->createBlendState(true,BLEND_SRCALPHA,BLEND_INVSRCALPHA);
}

ParticleMaterial::ParticleMaterial(const string& texFile, bool fileIsDir,
								   BlendMode srcBlend, BlendMode destBlend) : _textureSet(0)
{
	_device = ParticleEngine::getSingletonRef().getVideoDevice();

	if (texFile.length() > 0)
	{
		if (fileIsDir)		//  Texture set
			_textureSet = new ParticleTextureSet(texFile);
		else
			_texture = ParticleTextureStore::getSingletonRef().getTexture(texFile);
	}
	else
		_texture=0;

	_blendState = _device->createBlendState(true, srcBlend, destBlend);

//	if(!_texture->isGood())
//		_texture=ParticleTextureStore::getSingletonRef().getTexture("Textures/Default/Diffuse.bmp");
}

ParticleMaterial::ParticleMaterial(const vector<string>& texSet,
								   BlendMode srcBlend, BlendMode destBlend) : _texture(0)
{
	_device = ParticleEngine::getSingletonRef().getVideoDevice();

	_textureSet = new ParticleTextureSet;
	vector<string>::const_iterator t(texSet.begin());
	for(;t!=texSet.end();++t)
		_textureSet->addTexture(*t);

	_blendState = _device->createBlendState(true, srcBlend, destBlend);

	if(_textureSet->getTextureCount()==0)
	{
		delete _textureSet;		_textureSet=0;
//		_texture=ParticleTextureStore::getSingletonRef().getTexture("Textures/Default/Diffuse.bmp");
	}
}

ParticleMaterial::~ParticleMaterial()
{
	_device->destroyBlendState(_blendState);
	if(_texture)			ParticleTextureStore::getSingletonRef().releaseTexture(_texture->getName());
	if(_textureSet)			delete _textureSet;
}

BlendMode ParticleMaterial::getBlendMode(const string& bm) const
{
	BlendMode b=BLEND_ONE;
	for(int i=0;i<BLEND_COUNT;++i)
	{
		if(bm==g_ParticleBlendNames[i])
			b=(BlendMode)i;
	}

	return b;
}

ITexture* ParticleMaterial::getTexture(float percent) const
{
	if(_textureSet)
	{
		int tex=int((_textureSet->getTextureCount()-1) * percent);
		tex=smin(smax(tex,0),_textureSet->getTextureCount()-1);
		return _textureSet->getTexture(tex);
	}
	else
		return _texture;
}