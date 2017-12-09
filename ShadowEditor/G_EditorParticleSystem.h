
#ifndef _G_EDITORPARTICLESYSTEM_H_
#define _G_EDITORPARTICLESYSTEM_H_

#include <ParticleLib.h>

class G_EditorParticleSystem
{
protected:

	ParticleEmitter**				_emitters;
	int								_emitterCount;

	ParticleSystem*					_system;

	void							rebuildSystem();

public:

	G_EditorParticleSystem(const ParticleSystemTemplate& t);
	~G_EditorParticleSystem();

	ParticleSystem*					getSystem() const				{ return _system; }

	int								getEmitterCount() const			{ return _emitterCount; }
	ParticleEmitter*				getEmitter(int n) const			{ return _emitters[n]; }

	void							addEmitter(ParticleEmitter* e);
	void							remEmitter(int n);

	ParticleSystemTemplate*			buildTemplate();
};

#endif