
#include "stdafx.h"
#include "TraceTemplate.h"

TraceTemplate::TraceTemplate(ScriptFile& sf)
{
	string token(sf.getToken());
	while (sf.good() && token != "end_trace")
	{
		if (token == "particle_system")
			_particleSystem = new ParticleSystemTemplate(sf);
		else if (token == "density")
			_density = stof(sf.getToken());

		token = sf.getToken();
	}
}

TraceTemplate::~TraceTemplate()
{
	if (_particleSystem)
		delete _particleSystem;
}

