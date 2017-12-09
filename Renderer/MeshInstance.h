
#ifndef _MESHINSTANCE_H_
#define _MESHINSTANCE_H_

#include "Mesh.h"
#include "Renderable.h"
#include "IUpdatable.h"

class MeshInstance : public Renderable, public IUpdatable
{
protected:

	const Mesh*			_model;

public:

	MeshInstance(const Mesh *model, const Matrix4& world, bool alwaysVisible) : Renderable(model->getBBox(),world,alwaysVisible),
		_model(model)
	{
		if(model->getMaterial()->isAnimated())		wakeUp();
	}
	~MeshInstance()
	{
		if(_model->getMaterial()->isAnimated())		sleep();
	}

	const Mesh*		getModel() const					{ return _model; }

	void			enqueue();
};

#endif