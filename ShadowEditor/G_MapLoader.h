
#ifndef _G_MAPLOADER_H_
#define _G_MAPLOADER_H_

#include "G_Map.h"

class G_MapLoader
{
protected:

	void					parseToken(const string& token, ScriptFile& sf, G_Map& m) const;
	void					parseMaterial(ScriptFile& sf) const;
	void					parseRect(ScriptFile& sf, G_Map& m) const;
	void					parseGlobalLight(ScriptFile& sf) const;
	void					parseMesh(ScriptFile& sf, G_Map& m) const;
	void					parseParticles(ScriptFile& sf, G_Map& m) const;
	void					parseSound(ScriptFile& sf, G_Map& m) const;
	void					parseGameMaterial(ScriptFile& sf, G_Map& m) const;
	void					parseObject(ScriptFile& sf, G_Map& m) const;
	void					parseMeshObject(ScriptFile& sf, G_Map& m) const;
	void					parseParticlesObject(ScriptFile& sf, G_Map& m) const;
	void					parseSoundObject(ScriptFile& sf, G_Map& m) const;
	void					parseLight(ScriptFile& sf, G_Map& m) const;
	void					parseActor(ScriptFile& sf, G_Map& m) const;
	void					parseEntity(ScriptFile& sf, G_Map& m) const;
	void					parseGround(ScriptFile& sf, G_Map& m) const;

	G_SoundCone*			parseSoundCone(ScriptFile& sf) const;
	void					buildMatrix(const Vector3& rot, float x, float y, float height, Matrix4& world) const
	{
		Matrix4 rot_x, rot_y, rot_z;
		rot_x.createRotateX(rot.x * M_PI / 180.0f);
		rot_y.createRotateY(rot.y * M_PI / 180.0f);
		rot_z.createRotateZ(rot.z * M_PI / 180.0f);

		world = rot_z*rot_y;
		world *= rot_x;

		world(3, 0) = x;
		world(3, 1) = height;
		world(3, 2) = y;
	}


public:

	G_MapLoader()					{}

	G_Map*						loadMap(const string& mapName) const;
};

#endif