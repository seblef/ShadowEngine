
#include "stdafx.h"
#include "G_SoundTemplateSaver.h"

void G_SoundTemplateSaver::saveSounds(const G_ResourceCollector& c, ofstream& ofs) const
{
	G_ResourceCollector::SoundSet::const_iterator t(c.getSounds().begin());
	for (; t != c.getSounds().end(); ++t)
		saveTemplate(**t, ofs);
}

void G_SoundTemplateSaver::saveTemplate(G_SoundTemplate& s, ofstream& ofs) const
{
	ofs << "sound " << s.getName() << endl;

	ofs << "	priority	" << s.getPriority() << endl;
	ofs << "	radius	" << s.getRadius() << endl;
	ofs << "	radius_angle " << s.getRadiusAngle() << endl;
	ofs << "	file	" << s.getSoundFile() << endl;

	if (s.getCone())
		saveCone(*s.getCone(), ofs);

	ofs << "end_sound" << endl;
}

void G_SoundTemplateSaver::saveCone(const G_SoundCone& c, ofstream& ofs) const
{
	ofs << endl << "	cone" << endl;
	ofs << "		in_angle " << c.getInAngle() << endl << "		out_angle " << c.getOutAngle() << endl << endl;
	ofs << "		in_volume " << c.getInVolume() << endl << "		out_volume " << c.getOutVolume() << endl << endl;
	ofs << "		in_lpf " << c.getInLPF() << endl << "		out_lpf " << c.getOutLPF() << endl << endl;
	ofs << "		in_reverb " << c.getInReverb() << endl << "		out_reverb " << c.getOutReverb() << endl << endl;
	ofs << "	end_cone" << endl;
}