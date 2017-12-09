
#ifndef _G_MATERIALSAVE_H_
#define _G_MATERIALSAVE_H_

#include <StdRenderer.h>

class G_MaterialSave
{
protected:

	Material*					_mat;
	MaterialStdBuffer			_buffer;
	string						_textures[Material::TS_COUNT];
	unsigned int				_flags;
	bool						_blendEnable;
	BlendMode					_srcBlend;
	BlendMode					_destBlend;
	CullMode					_cullMode;

	TVector<MaterialAnimation>	_animations;

public:

	G_MaterialSave(Material* m);
	~G_MaterialSave()				{}

	void					restore();
};

#endif