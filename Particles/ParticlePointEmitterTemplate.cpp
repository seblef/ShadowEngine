
#include "stdafx.h"
#include "ParticlePointEmitterTemplate.h"

ParticlePointEmitterTemplate::ParticlePointEmitterTemplate(ScriptFile& sf) : ParticleEmitterTemplate("point")
{
	string token(sf.getToken());
	while (sf.good() && token != "end_emitter")
	{
		if (token == "common")
			parseCommon(sf);

		token = sf.getToken();
	}
}