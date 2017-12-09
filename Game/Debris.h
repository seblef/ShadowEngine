
#ifndef _DEBRIS_H_
#define _DEBRIS_H_

#include "Action.h"
#include "GameMaterial.h"
#include <StdRenderer.h>

class Debris : public Action
{
protected:

	class DebrisItem
	{
	public:

		MeshInstance*		_meshInstance;
		Vector3				_velocity;
		Vector3				_angleSpeed;
		Vector3				_position;
		Vector3				_rotation;
		float				_size;
	};

	int					_count;
	int					_alive;
	DebrisItem*			_items;

	void				updateItemMatrix(DebrisItem& d, Matrix4& w) const;

public:

	Debris(TemplateMesh* mesh, int count, const Vector3& pos,const Vector3& normal, float maxAngle,
		float minVelocity, float maxVelocity, float maxAngleSpeed, float size);
	~Debris();

	void				update(float time);
};

#endif