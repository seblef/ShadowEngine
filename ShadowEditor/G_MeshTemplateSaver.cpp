
#include "stdafx.h"
#include "G_MeshTemplateSaver.h"
#include "../Game/GameObject.h"


void G_MeshTemplateSaver::saveMeshes(const G_ResourceCollector& c, ofstream& ofs, bool hasGameMat) const
{
	G_ResourceCollector::MeshSet::const_iterator m(c.getMeshes().begin());
	for (; m != c.getMeshes().end(); ++m)
	{
		if ((hasGameMat && (*m)->getGameMaterial()) || (!hasGameMat && (*m)->getGameMaterial()==0))
			saveMesh(**m, ofs);
	}
}

void G_MeshTemplateSaver::saveMesh(const G_MeshTemplate& t, ofstream& ofs) const
{
	ofs << "mesh " << t.getName() << endl << endl;
	ofs << "	geometry " << t.getGeometryFile() << endl;
	ofs << "	material " << t.getMaterialName() << endl;
	ofs << "	physic_shape " << g_PhysicShapeNames[t.getPhysicShape()] << endl << endl;

	if(t.getFlag(OF_NOCOLLISION))
		ofs << "	no_collision" << endl;

	if (t.getGameMaterial())
		ofs << "	game_material " << t.getGameMaterial()->getName() << endl;

	ofs << endl << "end_mesh" << endl << endl;
}