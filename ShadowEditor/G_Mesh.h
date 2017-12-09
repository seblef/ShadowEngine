
#ifndef _G_MESH_H_
#define _G_MESH_H_

#include "G_Object.h"
#include "G_MeshTemplate.h"

class G_Mesh : public G_Object
{
protected:

	G_MeshTemplate*				_template;
	MeshInstance*				_rMesh;

	virtual void				rebuildMatrix();

public:

	G_Mesh(G_MeshTemplate* t);
	G_Mesh(const G_Mesh& m);
	~G_Mesh()
	{
		delete _rMesh;
	}

	G_MeshTemplate*				getTemplate() const					{ return _template; }

	virtual void				onAddToScene();
	virtual void				onRemFromScene();

	G_Object*					copy()								{ return new G_Mesh(*this); }
};

#endif