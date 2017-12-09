
#ifndef _MATERIALSYSTEM_H_
#define _MATERIALSYSTEM_H_

#include "Material.h"
#include <Core.h>

class MaterialSystem : public TSingleton<MaterialSystem>
{
protected:

	IVideoDevice*		_device;
	ITexture*			_defaultTexs[Material::TS_COUNT];
	IConstantBuffer*	_baseCB;
	SamplerState		_samplerState;

	typedef TDataBase<Material>		MaterialDB;
	MaterialDB			_materials;

public:

	MaterialSystem(IVideoDevice *device);
	~MaterialSystem();

	ITexture*			getDefaultTexture(Material::TextureSlot ts) const			{ return _defaultTexs[ts]; }
	IConstantBuffer*	getBaseConstantBuffer() const								{ return _baseCB; }

	Material*			loadMaterial(const string& fileName) const;
	Material*			loadMaterial(ScriptFile& file) const;

	void				setBuffers() const;

	void				registerMaterial(Material *m, const string& name)			{ _materials.registerData(name,m); }
	Material*			getMaterial(const string& name)								{ return _materials.getData(name); }
	void				clearMaterials()											{ _materials.clear(); 	registerMaterial(loadMaterial("default"),"default"); }

	TDataBase<Material>&	getMaterials()											{ return _materials; }
};

#endif