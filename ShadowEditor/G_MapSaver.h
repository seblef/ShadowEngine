
#ifndef _G_MAPSAVER_H_
#define _G_MAPSAVER_H_

#include "G_IMapVisitor.h"
#include "G_Map.h"

class G_MapSaver : public G_IMapVisitor
{
protected:

	G_Map*				_map;
	ofstream*			_ofs;

	void				outputVector(const string& n, const Vector3& v) const
	{
		(*_ofs) << "	" << n << " " << v.x << " " << v.y << " " << v.z << endl;
	}

	void				outputColor(const Color& c) const
	{
		(*_ofs) << c.r << " " << c.g << " " << c.b << " " << c.a;
	}

	void				saveMesh(G_Object *o) const;
	void				saveLight(G_Object* o) const;
	void				saveParticleSystem(G_Object* o) const;
	void				saveSound(G_Object* o) const;
	void				saveActor(G_Object *o) const;
	void				saveEntity(G_Object* o) const;

public:

	G_MapSaver(G_Map *m) : _map(m)			{}

	void				saveMap();

	void				visitGround(G_Ground* g);
	void				visitObject(G_Object* o);
};

#endif