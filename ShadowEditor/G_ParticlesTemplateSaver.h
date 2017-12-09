
#ifndef _G_PARTICLESTEMPLATESAVER_H_
#define _G_PARTICLESTEMPLATESAVER_H_

#include "G_ResourceCollector.h"

class G_ParticlesTemplateSaver
{
protected:

	void				saveEmitter(ParticleEmitterTemplate* et, ofstream& ofs) const;

public:

	G_ParticlesTemplateSaver()				{}

	void				saveTemplate(ParticleSystemTemplate& t, ofstream& ofs, const string& name) const;
	void				saveParticleSystems(const G_ResourceCollector& c, ofstream& ofs) const;
};

#endif