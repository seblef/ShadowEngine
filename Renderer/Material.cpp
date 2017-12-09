
#include "stdafx.h"
#include "Material.h"
#include "MaterialAnimationFactory.h"
#include "MaterialSystem.h"

extern ofstream g_MediaOut;

Material::Material(IVideoDevice *d) : _device(d), _flags(0)
{
	for(int i=0;i<TS_COUNT;++i)
	{
		_textures[i]=MaterialSystem::getSingletonRef().getDefaultTexture((TextureSlot)i);
		_defaultTex[i]=true;
	}

	_renderState=d->createRenderState(CULL_BACK);
	_blendState=d->createBlendState(false,BLEND_ONE,BLEND_ZERO);
}

Material::Material(ScriptFile& file, IVideoDevice* device) : _renderState(0), _blendState(0), _device(device), 
	_flags(0)
{
	for(int i=0;i<TS_COUNT;++i)
	{
		_textures[i]=MaterialSystem::getSingletonRef().getDefaultTexture((TextureSlot)i);
		_defaultTex[i]=true;
	}

	string token(file.getToken());
	while(file.good() && token!=_T("end_material"))
	{
		parseToken(token,file);
		token=file.getToken();
	}

	if((!_defaultTex[TS_ENVIONMENT]) || (!_defaultTex[TS_EMISSIVE]))
		setFlag(MF_ADDPASS);

	if(!_renderState)			_renderState=device->createRenderState(CULL_BACK);
	if(!_blendState)			_blendState=device->createBlendState(false,BLEND_ONE,BLEND_ZERO);
}

Material::~Material()
{
	_device->destroyBlendState(_blendState);
	_device->destroyRenderState(_renderState);

	for(int i=0;i<TS_COUNT;++i)
	{
		if(!_defaultTex[i])
			delete _textures[i];
	}
}

BlendMode Material::getBlendMode(const string& b) const
{
	BlendMode m=BLEND_ZERO;
	for(int i=0;i<BLEND_COUNT;++i)
	{
		if(b==g_MaterialBlendNames[i])
			m=(BlendMode)i;
	}

	return m;
}

Material::TextureSlot Material::getTextureSlot(const string& b) const
{
	TextureSlot t=TS_DIFFUSE;
	for(int i=0;i<TS_COUNT;++i)
	{
		if(b==g_MaterialTexNames[i])
			t=(TextureSlot)i;
	}

	return t;
}

CullMode Material::getCullMode(const string& c) const
{
	CullMode cm=CULL_BACK;
	if(c==_T("none"))			cm=CULL_NONE;
	else if(c==_T("front"))		cm=CULL_FRONT;

	return cm;
}

Vector2 Material::parseVector2(ScriptFile& fl) const
{
	Vector2 v;
	v.x=stof(fl.getToken());
	v.y=stof(fl.getToken());
	return v;
}

bool Material::parseToken(const string& token, ScriptFile& file)
{
	Color c;

	if(token==_T("tex"))
	{
		TextureSlot stage=getTextureSlot(file.getToken());
		_textures[stage]=_device->createTexture(file.getToken());
		_defaultTex[stage]=false;
	}
	else if(token==_T("ambient"))
	{
		file.parseColor(c);
		_buffer.setAmbient(c);
	}
	else if(token==_T("diffuse"))
	{
		file.parseColor(c);
		_buffer.setDiffuse(c);
	}
	else if(token==_T("specular"))
	{
		file.parseColor(c);
		_buffer.setSpecular(c);
	}
	else if(token==_T("shine"))
		_buffer.setShininess(stof(file.getToken()));
	else if(token==_T("spec_intens"))
		_buffer.setSpecIntensity(stof(file.getToken()));
	else if(token==_T("blend"))
	{
		BlendMode src=getBlendMode(file.getToken());
		BlendMode dest=getBlendMode(file.getToken());
		_blendState=_device->createBlendState(true,src,dest);
	}
	else if(token==_T("alpha_material"))
		setFlag(MF_ALPHA);
	else if(token==_T("animation"))
		_anims.getVector().push_back(MaterialAnimationFactory::parseAnimation(file,this));
	else if(token==_T("cull"))
		_renderState=_device->createRenderState(getCullMode(file.getToken()));
	else if(token==_T("uv_offset"))
		_buffer.setOffset(parseVector2(file));
	else if(token==_T("uv_scale"))
		_buffer.setScale(parseVector2(file));
	else if(token==_T("uv_rotation"))
	{
		float r=stof(file.getToken()) * M_PI / 180.0f;
		Matrix2 rot;
		rot.rotation(r);
		_buffer.setRotation(rot);
	}
	else if(token==_T("cast_no_shadows"))
		setFlag(MF_CASTNOSHADOWS);
	else
		return false;

	return true;
}

void Material::setBase(float time)
{
	for(int i=0;i<=TS_SPECULAR;++i)
		_textures[i]->set(i);

	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);

	setTime(time);
}

void Material::setAdd(float time)
{
	_textures[TS_EMISSIVE]->set(0);
	_textures[TS_ENVIONMENT]->set(1);

	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);

	setTime(time);
}

void Material::setTime(float time)
{
	vector<MaterialAnimation*>::iterator a(_anims.getVector().begin());
	for(;a!=_anims.getVector().end();++a)
		(*a)->set(time);

	MaterialSystem::getSingletonRef().getBaseConstantBuffer()->fill((const void*)&_buffer);
}

void Material::createAnimation(const string& animName)
{
	_anims.getVector().push_back(MaterialAnimationFactory::createAnimation(animName,this));
}

void Material::deleteAnimation(int n)
{
	vector<MaterialAnimation*>::iterator a(_anims.getVector().begin()+n);
	delete *a;
	_anims.getVector().erase(a);
}

void Material::setTexture(TextureSlot s, const string& texFile)
{
	if(!_defaultTex[s])
	{
		delete _textures[s];
		_textures[s]=MaterialSystem::getSingletonRef().getDefaultTexture(s);
		_defaultTex[s]=true;
	}

	if(texFile.length() > 0)
	{
		_textures[s]=_device->createTexture(texFile);
		_defaultTex[s]=false;
	}
}

void Material::updateStdBuffer()
{
	MaterialSystem::getSingletonRef().getBaseConstantBuffer()->fill((const void*)&_buffer);
}