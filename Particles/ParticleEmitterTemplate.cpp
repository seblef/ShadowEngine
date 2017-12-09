
#include "stdafx.h"
#include "ParticleEmitterTemplate.h"

ParticleEmitterTemplate::~ParticleEmitterTemplate()
{
	if (_material)		delete _material;
}

void ParticleEmitterTemplate::parseCommon(ScriptFile& sf)
{
	string token(sf.getToken());
	while (sf.good() && token != "end_common")
	{
		if (token == "life")
		{
			_params1.setLife(stof(sf.getToken()));
			_params2.setLife(stof(sf.getToken()));
		}
		else if (token == "mass")
		{
			_params1.setMass(stof(sf.getToken()));
			_params2.setMass(stof(sf.getToken()));
		}
		else if (token == "friction")
		{
			_params1.setFriction(stof(sf.getToken()));
			_params2.setFriction(stof(sf.getToken()));
		}
		else if (token == "velocity")
		{
			Vector3 v;
			sf.parseVector(v);
			_params1.setVelocity(v);
			sf.parseVector(v);
			_params2.setVelocity(v);
		}
		else if (token == "start_color")
		{
			Color c;
			sf.parseColor(c);
			_params1.setStartColor(c);
			sf.parseColor(c);
			_params2.setStartColor(c);
		}
		else if (token == "end_color")
		{
			Color c;
			sf.parseColor(c);
			_params1.setEndColor(c);
			sf.parseColor(c);
			_params2.setEndColor(c);
		}
		else if (token == "start_size")
		{
			Vector3 s;
			sf.parseVector(s);
			_params1.setStartSize(s);
			sf.parseVector(s);
			_params2.setStartSize(s);
		}
		else if (token == "end_size")
		{
			Vector3 s;
			sf.parseVector(s);
			_params1.setEndSize(s);
			sf.parseVector(s);
			_params2.setEndSize(s);
		}
		else if (token == "material")
		{
			if (_material)		delete _material;
			_material = new ParticleMaterial(sf);
		}
		else if (token == "renderer")
			_renderer = sf.getToken();
		else if (token == "max_particles")
			_maxParticles = stoi(sf.getToken());
		else if (token == "gravity")
			sf.parseVector(_gravity);
		else if (token == "emission_rate")
			_emissionRate = stof(sf.getToken());

		token = sf.getToken();
	}
}