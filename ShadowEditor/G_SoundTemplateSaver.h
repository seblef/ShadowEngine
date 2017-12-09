
#ifndef _G_SOUNDTEMPLATESAVER_H_
#define _G_SOUNDTEMPLATESAVER_H_

#include "G_ResourceCollector.h"

class G_SoundTemplateSaver
{
protected:

	void				saveCone(const G_SoundCone& c, ofstream& ofs) const;
	void				saveTemplate(G_SoundTemplate& s, ofstream& ofs) const;

public:

	G_SoundTemplateSaver()				{}

	void				saveSounds(const G_ResourceCollector& c, ofstream& ofs) const;
};

#endif