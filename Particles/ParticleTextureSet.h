
#ifndef _PARTICLETEXTURESET_H_
#define _PARTICLETEXTURESET_H_

#include <MediaCommon.h>


class ParticleTextureSet
{
	protected:

		typedef vector<ITexture*>		TexVector;
		TexVector						_textures;

	public:

		ParticleTextureSet()			{}
		ParticleTextureSet(const ParticleTextureSet& s);
		ParticleTextureSet(ScriptFile& sf);
		ParticleTextureSet(const string& dir);
		~ParticleTextureSet();

		void							addTexture(const string& texFile);
		int								getTextureCount() const					{ return _textures.size(); }
		ITexture*						getTexture(const int n) const			{ return _textures[n]; }
		ITexture*						operator[](size_t n) const				{ return getTexture((int)n); }
};

#endif