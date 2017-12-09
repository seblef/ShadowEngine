
#include "stdafx.h"
#include "G_GameMaterialSaver.h"

void G_GameMaterialSaver::saveMaterials(const G_ResourceCollector& c, ofstream& ofs) const
{
	G_ResourceCollector::GameMaterialSet::const_iterator m(c.getGameMaterials().begin());
	for (; m != c.getGameMaterials().end(); ++m)
		saveMaterial(**m, ofs);
}

void G_GameMaterialSaver::saveMaterial(const G_GameMaterial& m, ofstream& ofs) const
{
	ofs << "game_material " << m.getName() << endl;

	if (m.getDebris())
		ofs << "	debris " << m.getDebris()->getName() << endl;

	ofs << "end_game_material" << endl << endl;
}