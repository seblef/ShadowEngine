
#ifndef _GAMEMESH_H_
#define _GAMEMESH_H_

#include "TemplateMesh.h"
#include "GameObject.h"

class GameMesh : public GameObject
{
protected:

	MeshInstance*				_rInstance;
	IPhysicObject*				_physic;
	const TemplateMesh*			_template;

public:

	GameMesh(const TemplateMesh& t, const Matrix4& world, bool alwaysVisible=false);
	~GameMesh();

	void						onAddToScene();
	void						onRemFromScene();
	void						onUpdateMatrix(const Matrix4& new_m)	{}

	const TemplateMesh*			getTemplate() const					{ return _template; }
};

#endif