
#include "stdafx.h"
#include "Map.h"
#include "BBoxToRect.h"

extern ofstream			g_MediaOut;

Map::Map(int w, int h) : _nullGameMaterial(0), _ground(w,h, &_nullGameMaterial), _startPosition(5,5)
{
	Renderer::getSingletonRef().getVisibilitySystem()->create(w,h);

	PhysicGeometryCreate_t c;
	c._box.setMin(Vector3(0, -1, 0));
	c._box.setMax(Vector3((float)w, 0, (float)h));
	c._shape = PSHAPE_BOX;
	PhysicGeometry* geo = Physic::getSingletonRef().createGeometry(c);
	_pObject = Physic::getSingletonRef().createStaticScene(*geo, Matrix4::Identity);
	_pObject->addToScene();


//	_pObject = Physic::getSingletonRef().createGround(w, h);
//	_pObject->addToScene();
}

Map::~Map()
{
	_pObject->remFromScene();
	delete _pObject;

	_staticObjects.clear();
	_tempObjects.clear();

	MaterialSystem::getSingletonRef().clearMaterials();
}

void Map::finalize()
{
	_ground.finalize();
}

void Map::addObject(GameObject* o, bool temp)
{
	if(temp)		_tempObjects.getSet().insert(o);
	else			_staticObjects.getVector().push_back(o);

	o->onAddToScene();
}

void Map::outputWakableInfo() const
{
	for(int y=0;y<_ground.getHeight();++y)
	{
		for(int x=0;x<_ground.getWidth();++x)
			g_MediaOut << (_ground.isCaseWakable(x,y) ? "0 " : "1 ");

		g_MediaOut << endl;
	}
}