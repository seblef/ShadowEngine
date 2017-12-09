
#ifndef _G_ENTITY_H_
#define _G_ENTITY_H_

#include "G_Object.h"
#include "G_EntityTemplate.h"

class G_Entity : public G_Object
{
protected:

	G_EntityTemplate*			_template;
	MeshInstance*				_rMesh;

	virtual void				rebuildMatrix();

public:

	G_Entity(G_EntityTemplate* t);
	G_Entity(const G_Entity& e);
	~G_Entity();

	G_EntityTemplate*			getTemplate() const					{ return _template; }

	virtual void				onAddToScene();
	virtual void				onRemFromScene();

	G_Object*					copy()								{ return new G_Entity(*this); }

	void						drawSelection(G_EditorObjDrawer* d);
};


#endif