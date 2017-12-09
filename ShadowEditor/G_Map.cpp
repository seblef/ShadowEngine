
#include "stdafx.h"
#include "G_Map.h"

G_Map::G_Map(int w,int h) : _name("new_map"), _startPosition(Vector2::UnitVector)
{
	Material* m=MaterialSystem::getSingletonRef().getMaterial("default");
	int *ids=new int[w*h];
	for(int i=0;i<w*h;++i)
		ids[i]=-1;

	G_GameMaterial *nullMat = new G_GameMaterial("null", 0);
	_gameMaterials.registerData("null", nullMat);
	_ground=new G_Ground(w,h,1,&m,ids,nullMat);
	Renderer::getSingletonRef().setGroundData(_ground);

	delete[] ids;
}

G_Map::~G_Map()
{
	delete _ground;

	ObjectSet::iterator o(_objects.begin());
	for(;o!=_objects.end();++o)
	{
		(*o)->onRemFromScene();
		delete *o;
	}
}

void G_Map::visit(G_IMapVisitor* v)
{
	v->visitGround(_ground);

	ObjectSet::iterator o(_objects.begin());
	for(;o!=_objects.end();++o)
		v->visitObject(*o);
}