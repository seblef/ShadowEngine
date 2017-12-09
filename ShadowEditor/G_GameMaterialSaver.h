
#ifndef _G_GAMEMATERIALSAVER_H_
#define _G_GAMEMATERIALSAVER_H_

#include "G_ResourceCollector.h"

class G_GameMaterialSaver
{
protected:

	void			saveMaterial(const G_GameMaterial& m, ofstream& ofs) const;

public:

	G_GameMaterialSaver()			{}

	void			saveMaterials(const G_ResourceCollector& c, ofstream& ofs) const;
};

#endif