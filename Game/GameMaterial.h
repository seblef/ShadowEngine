
#ifndef _GAMEMATERIAL_H_
#define _GAMEMATERIAL_H_

#include "TemplateMesh.h"

class GameMaterial
{
protected:

	TemplateMesh*				_debrisTemplate;

public:

	GameMaterial(TemplateMesh* debris) : _debrisTemplate(debris)			{}
	~GameMaterial();

	TemplateMesh*				getDebrisTemplate() const					{ return _debrisTemplate; }
};

#endif