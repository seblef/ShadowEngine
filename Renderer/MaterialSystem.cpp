
#include "stdafx.h"
#include "MaterialSystem.h"

extern ofstream		g_MediaOut;

const char* const g_MaterialDefaultTexs[Material::TS_COUNT]={
	_T("Textures/Default/Diffuse.bmp"),
	_T("Textures/Default/Normal.bmp"),
	_T("Textures/Default/Specular.bmp"),
	_T("Textures/Default/Emissive.bmp"),
	_T("Textures/Default/Environment.bmp")
};

MaterialSystem::MaterialSystem(IVideoDevice* device) : _device(device)
{
	for(int i=0;i<Material::TS_COUNT;++i)
		_defaultTexs[i]=device->createTexture(g_MaterialDefaultTexs[i]);

	_samplerState=device->createSamplerState(FILTER_TRILINEAR);
	_baseCB=device->createConstantBuffer(6,2);
	registerMaterial(loadMaterial("default"),"default");
}

MaterialSystem::~MaterialSystem()
{
	_device->destroySamplerState(_samplerState);
	delete _baseCB;

	for(int i=0;i<Material::TS_COUNT;++i)
		delete _defaultTexs[i];
}

Material* MaterialSystem::loadMaterial(const string& fileName) const
{
	string file(_T("Materials\\"));
	file+=fileName;
	file+=_T(".txt");

	ScriptFile sf(file);
	sf.getToken();		// Material keyword
	sf.getToken();		// Material name
	sf.getToken();		// Material type

	return new Material(sf,_device);
}

Material* MaterialSystem::loadMaterial(ScriptFile& file) const
{
	file.getToken();		// Material type
	return new Material(file,_device);
}

void MaterialSystem::setBuffers() const
{
	_baseCB->set(ST_PIXEL);
	_baseCB->set(ST_VERTEX);

	SamplerState ss[4]={
		_samplerState,
		_samplerState,
		_samplerState,
		_samplerState
	};

	_device->setSamplerState(0,4,ss);
}