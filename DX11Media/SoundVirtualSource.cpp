
#include "stdafx.h"
#include "SoundVirtualSource.h"

void DXSoundVirtualSource::setCone(ISoundCone *c)
{
	if (c)
	{
		_emitter.pCone = ((SoundCone*)c)->getCone();
		_cone = (SoundCone*)c;
	}
	else
	{
		_emitter.pCone = 0;
		_cone = 0;
	}
}

#ifdef _DEBUG

void DXSoundVirtualSource::outputSource(ofstream& ofs) const
{
	ofs << "-- Source --" << endl;
	ofs << "	Cone: " << (_emitter.pCone ? "true" : "false") << endl;
	ofs << "	Position: " << SOUND_RETURN_VEC_CONST(_emitter.Position) << endl;
	ofs << "	Velocity: " << SOUND_RETURN_VEC_CONST(_emitter.Velocity) << endl;
	ofs << "	Radius: " << _emitter.InnerRadius << endl;
	ofs << "	Radius Angle: " << _emitter.InnerRadiusAngle << endl;
	ofs << "-- End Source--" << endl << endl;
}

#endif