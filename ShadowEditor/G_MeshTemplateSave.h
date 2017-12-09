
#ifndef _G_MESHTEMPLATESAVE_H_
#define _G_MESHTEMPLATESAVE_H_

#include "G_MeshTemplate.h"

class G_MeshTemplateSave
{
protected:

	G_MeshTemplate*			_mesh;
	G_GameMaterial*			_gameMaterial;
	string					_material;
	string					_name;
	PhysicShape				_shape;
	unsigned int			_flags;

public:

	G_MeshTemplateSave(G_MeshTemplate* t) : _mesh(t), _material(t->getMaterialName()),
		_name(t->getName()), _shape(t->getPhysicShape()), _flags(t->getFlag(0xffffffff)),
		_gameMaterial(t->getGameMaterial()) {}

	void					restore()
	{
		_mesh->setMaterial(_material);
		_mesh->setName(_name);
		_mesh->setPhysicShape(_shape);
		_mesh->unsetFlag(0xffffffff);
		_mesh->setFlag(_flags);
		_mesh->setGameMaterial(_gameMaterial);
	}
};

#endif