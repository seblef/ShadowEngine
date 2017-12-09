
#ifndef _G_GAMEMATERIAL_H_
#define _G_GAMEMATERIAL_H_

#include "G_MeshTemplate.h"

class G_GameMaterial
{
protected:

	string						_name;
	G_MeshTemplate*				_debris;

public:

	G_GameMaterial(const string& name, G_MeshTemplate* debris) : _name(name), _debris(debris)			{}
	~G_GameMaterial()																					{}

	void						setName(const string& name)				{ _name = name; }
	void						setDebris(G_MeshTemplate* debris)		{ _debris = debris; }

	const string&				getName() const							{ return _name; }
	G_MeshTemplate*				getDebris() const						{ return _debris; }
};

#endif