
#include "stdafx.h"
#include "Debris.h"
#include "SystemValues.h"

extern ofstream g_MediaOut;

Debris::Debris(TemplateMesh* mesh, int count, const Vector3& pos, const Vector3& normal, float maxAngle,
	float minVelocity, float maxVelocity, float maxAngleSpeed, float size) : _count(count), _alive(count)
{
	assert(count > 0 && mesh);

	Vector3 maxRot(2.0f * M_PI, 2.0f * M_PI, 2.0f * M_PI);
	Matrix4 rot, world;

	_items = new DebrisItem[count];
	for (int i = 0; i < count; ++i)
	{
		DebrisItem& d(_items[i]);
		d._position = pos;
		d._size = size;
		randomVector(d._rotation, Vector3::NullVector, maxRot);
		d._angleSpeed.x = randomScalar(0.0f, maxAngleSpeed);
		d._angleSpeed.y = randomScalar(0.0f, maxAngleSpeed);
		d._angleSpeed.z = randomScalar(0.0f, maxAngleSpeed);

		float rx = randomScalar(-maxAngle, maxAngle);
		float ry = randomScalar(-maxAngle, maxAngle);
		float rz = randomScalar(-maxAngle, maxAngle);

		//			Can be optimized
		rot.createRotateXYZ(rx, ry, rz);
		d._velocity = normal;
		d._velocity.transformNoTranslation(rot);
		d._velocity *= randomScalar(minVelocity, maxVelocity);

		updateItemMatrix(d, world);
		d._meshInstance = new MeshInstance(&mesh->getMesh(), world, true);
		Renderer::getSingletonRef().addRenderable(d._meshInstance);
	}
}

Debris::~Debris()
{
	for (int i = 0; i < _count; ++i)
	{
		if (_items[i]._position.y > -1.0f)
			Renderer::getSingletonRef().remRenderable(_items[i]._meshInstance);

		delete _items[i]._meshInstance;
	}

	delete[] _items;
}

void Debris::update(float time)
{
	Vector3 g(SystemValues::getSingletonRef().getGravity() * time);
	Matrix4 world;

	for (int i = 0; i < _count; ++i)
	{
		DebrisItem& d = _items[i];
		if (d._position.y > -1.0f)
		{
			d._velocity += g;
			d._position += d._velocity * time;

			if (d._position.y < -1.0f)
			{
				--_alive;
				Renderer::getSingletonRef().remRenderable(d._meshInstance);
			}
			else
			{
				d._rotation += d._angleSpeed * time;

				updateItemMatrix(d, world);
				d._meshInstance->setWorldMatrix(world);
			}
		}
	}

	_dead = (_alive == 0);
}

void Debris::updateItemMatrix(DebrisItem& d, Matrix4& m) const
{
	Matrix4 rot_x, rot_y, rot_z;
	rot_x.createRotateX(d._rotation.x);
	rot_y.createRotateY(d._rotation.y);
	rot_z.createRotateZ(d._rotation.z);

	m.createScale(d._size, d._size, d._size);
	m*= rot_z*rot_y;
	m*= rot_x;

	m(3, 0) = d._position.x;
	m(3, 1) = d._position.y;
	m(3, 2) = d._position.z;
}