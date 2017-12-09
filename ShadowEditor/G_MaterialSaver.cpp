
#include "stdafx.h"
#include "G_MaterialSaver.h"
#include "G_EditorApp.h"

void G_MaterialSaver::saveMaterials(const G_ResourceCollector& c, ofstream& ofs) const
{
	string matName;
	const map<string,Material*>& db(MaterialSystem::getSingletonRef().getMaterials().getData());
	map<string,Material*>::const_iterator m;

	G_ResourceCollector::MaterialSet::const_iterator mat(c.getMaterials().begin());

	for(;mat!=c.getMaterials().end();++mat)
	{
		for(m=db.begin();m!=db.end();++m)
			if(m->second==*mat)
				matName=m->first;

		saveMaterial(**mat,matName,ofs);
	}
}

void G_MaterialSaver::saveMaterial(const Material& m, const string& matName, ofstream& ofs) const
{
	ofs << "material " << matName << " std" << endl << endl;

	const MaterialStdBuffer& b(m.getStdBuffer());
	//				Colors
	outputColor("ambient",b.getAmbient(),ofs);
	outputColor("diffuse",b.getDiffuse(),ofs);
	outputColor("specular",b.getSpecular(),ofs);
	ofs << endl << "	shine " << b.getShininess() << endl << "	spec_intens " << b.getSpecIntensity() << endl << endl;

	//				Textures
	for(int i=0;i<Material::TS_COUNT;++i)
	{
		Material::TextureSlot ts=(Material::TextureSlot)i;
		if(!m.isDefaultTexture(ts))
			ofs << "	tex " << g_MaterialTexNames[i] << " " << m.getTexture(ts)->getName() << endl;
	}

	//				UV transforms
	ofs << endl;
	if(b.getOffset()!=Vector2::NullVector)							outputVector("uv_offset",b.getOffset(),ofs);
	if(b.getScale()!=Vector2::UnitVector)							outputVector("uv_scale",b.getScale(),ofs);
	if(b.getRotation()(0,0)!=1.0f || b.getRotation()(0,1)!=0.0f)
	{
		float r=acosf(b.getRotation()(0,0)) * 180.0f / M_PI;
		if(b.getRotation()(0,1) < 0.0f)
			r=-r;

		ofs << "	uv_rotation " << r << endl;
	}

	ofs << endl;

	for(int i=0;i<m.getAnimationCount();++i)
	{
		MaterialAnimation* a=m.getAnimation(i);
		if(strcmp(a->getAnimationName(),"uv")==0)				saveUVAnim((MaterialUVAnimation*)a,ofs);
		else if(strcmp(a->getAnimationName(),"texture")==0)		saveTextureAnim((MaterialTextureAnimation*)a,ofs);
		else if(strcmp(a->getAnimationName(),"fade")==0)		saveFadeAnim((MaterialFadeAnimation*)a,ofs);
	}

	ofs << endl;

	//					Render states
	bool blendEnable;
	BlendMode src,dest;
	FillMode fm;
	CullMode cm;
	G_EditorApp::getSingletonRef().getVideo()->getBlendStateDesc(m.getBlendState(),blendEnable,src,dest);
	G_EditorApp::getSingletonRef().getVideo()->getRenderStateDesc(m.getRenderState(),cm,fm);

	if(blendEnable)
		ofs << "	blend " << g_MaterialBlendNames[src] << " " << g_MaterialBlendNames[dest] << endl;

	if(cm!=CULL_BACK)
		ofs << "	cull " << (cm==CULL_FRONT ?  "front" : "none") << endl;

	//					Flags
	ofs << endl;
	if(m.getFlag(MF_ALPHA))					ofs << "	alpha_material" << endl;
	if(m.getFlag(MF_CASTNOSHADOWS))			ofs << "	cast_no_shadows" << endl;

	ofs << endl << "end_material" << endl << endl;
}

void G_MaterialSaver::saveUVAnim(MaterialUVAnimation* a, ofstream& ofs) const
{
	ofs << "animation uv" << endl << "	offset " << a->getOffset().x << " " << a->getOffset().y << endl << "	rotation " << a->getRotation() << endl << "end_anim" << endl;
}

void G_MaterialSaver::saveTextureAnim(MaterialTextureAnimation* a, ofstream& ofs) const
{
	ofs << "animation texture " << a->getAnimTime() << " " << a->getTileCount() << " " << a->getTilePerRow() << " " << a->getTilePerColumn() << endl;
}

void G_MaterialSaver::saveFadeAnim(MaterialFadeAnimation* a, ofstream& ofs) const
{
	ofs << "animation fade " << a->getStartTime() << " " << a->getEndTime() << endl;
}