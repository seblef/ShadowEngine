
#include "stdafx.h"
#include "MaterialAnimationFactory.h"
#include "MaterialTextureAnimation.h"
#include "MaterialUVAnimation.h"
#include "MaterialFadeAnimation.h"


MaterialAnimation* MaterialAnimationFactory::createAnimation(const string& animName, Material* m)
{
	if(animName=="uv")				return new MaterialUVAnimation(m);
	else if(animName=="texture")	return new MaterialTextureAnimation(m);
	else if(animName=="fade")		return new MaterialFadeAnimation(m);
	else							return 0;
}

MaterialAnimation* MaterialAnimationFactory::parseAnimation(ScriptFile& sf, Material *m)
{
	const string& name(sf.getToken());
	if(name=="uv")					return new MaterialUVAnimation(m,sf);
	else if(name=="texture")		return new MaterialTextureAnimation(m,sf);
	else if(name=="fade")			return new MaterialFadeAnimation(m,sf);
	else							return 0;
}

MaterialAnimation* MaterialAnimationFactory::copyAnimation(const MaterialAnimation* a)
{
	const string& name(a->getAnimationName());
	if(name=="uv")					return new MaterialUVAnimation(*((MaterialUVAnimation*)a));
	else if(name=="texture")		return new MaterialTextureAnimation(*((MaterialTextureAnimation*)a));
	else if(name=="fade")			return new MaterialFadeAnimation(*((MaterialFadeAnimation*)a));
	else							return 0;
}