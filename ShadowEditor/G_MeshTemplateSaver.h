
#ifndef _G_MESHTEMPLATESAVER_H_
#define _G_MESHTEMPLATESAVER_H_

#include "G_ResourceCollector.h"

class G_MeshTemplateSaver
{
protected:

	void						saveMesh(const G_MeshTemplate& t, ofstream& ofs) const;

public:

	G_MeshTemplateSaver()		{}

	void						saveMeshes(const G_ResourceCollector& c, ofstream& ofs, bool hasGameMat) const;
};

#endif