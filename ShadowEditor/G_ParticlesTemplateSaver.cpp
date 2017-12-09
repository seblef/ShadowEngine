
#include "stdafx.h"
#include "G_ParticlesTemplateSaver.h"
#include "G_EditorApp.h"

void G_ParticlesTemplateSaver::saveParticleSystems(const G_ResourceCollector& c, ofstream& ofs) const
{
	G_ResourceCollector::ParticleSet::const_iterator t(c.getParticles().begin());
	for (; t != c.getParticles().end(); ++t)
		saveTemplate((*t)->getTemplate(), ofs, (*t)->getName());
}

void G_ParticlesTemplateSaver::saveTemplate(ParticleSystemTemplate& ps, ofstream& ofs,
	const string& name) const
{
	ofs << "particle_system " << name << endl;

	for (int i = 0; i < ps.getEmitterCount(); ++i)
		saveEmitter(ps.getEmitter(i), ofs);

	ofs << "end_system" << endl;
}

void G_ParticlesTemplateSaver::saveEmitter(ParticleEmitterTemplate* et, ofstream& ofs) const
{
	ofs << "emitter " << et->getEmitterName() << endl << "	common" << endl;

	const ParticleEmitterParams& p1(et->getParameters1());
	const ParticleEmitterParams& p2(et->getParameters2());

#define OVEC(v)		v.x << " " << v.y << " " << v.z
#define OCOL(c)		c.r << " " << c.g << " " << c.b << " " << c.a

	ofs << "	life " << p1.getLife() << " " << p2.getLife() << endl;
	ofs << "	mass " << p1.getMass() << " " << p2.getMass() << endl;
	ofs << "	friction " << p1.getFriction() << " " << p2.getFriction() << endl;

	ofs << "	velocity " << OVEC(p1.getVelocity()) << "	" << OVEC(p2.getVelocity()) << endl;
	
	ofs << "	start_color " << OCOL(p1.getStartColor()) << "	" << OCOL(p2.getStartColor()) << endl;
	ofs << "	end_color " << OCOL(p1.getEndColor()) << "	" << OCOL(p2.getEndColor()) << endl;

	ofs << "	start_size " << OVEC(p1.getStartSize()) << "	" << OVEC(p2.getStartSize()) << endl;
	ofs << "	end_size " << OVEC(p1.getEndSize()) << "	" << OVEC(p2.getEndSize()) << endl;

	ofs << "	gravity " << OVEC(et->getGravity()) << endl;
	ofs << "	renderer " << et->getRenderer() << endl;

	ofs << "	max_particles " << et->getMaxParticles() << endl;
	ofs << "	emission_rate " << et->getEmissionRate() << endl;

	ParticleMaterial *m = et->getMaterial();
	ofs << "	material" << endl;

	ParticleTextureSet *s = m->getTextureSet();

	if (s)
	{
		ofs << "		texture_set" << endl;
		for (int i = 0; i < s->getTextureCount(); ++i)
			ofs << "		" << s->getTexture(i)->getName() << endl;
		ofs << "		end_texture_set" << endl;
	}
	else if (m->getTexture())
		ofs << "		texture " << m->getTexture()->getName() << endl;

	BlendMode src, dest;
	bool blend;
	G_EditorApp::getSingletonRef().getVideo()->getBlendStateDesc(m->getBlendState(), blend, src, dest);
	if (blend != true || src != BLEND_SRCALPHA || dest != BLEND_INVSRCALPHA)
		ofs << "		blend " << g_MaterialBlendNames[src] << " " << g_MaterialBlendNames[dest] << endl;

	ofs << "	end_material" << endl;

	ofs << "	end_common" << endl;

	//		[TODO]: Add specialized saving

	ofs << "end_emitter" << endl << endl;
}