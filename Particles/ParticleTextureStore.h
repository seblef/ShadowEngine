
#ifndef _PARTICLETEXTURESTORE_H_
#define _PARTICLETEXTURESTORE_H_

#include <MediaCommon.h>

using namespace Core;
using namespace std;

class ParticleTextureStore : public TSingleton<ParticleTextureStore>
{
	private:
		class PTSItem
		{
			private:

				int				_refCount;
				ITexture*		_texture;

			public:

				PTSItem() : _refCount(1), _texture(0)		{}
				PTSItem(const PTSItem& t) : _refCount(t._refCount), _texture(t._texture)	{}
				PTSItem(ITexture* t) : _refCount(1), _texture(t)			{}
				~PTSItem()
				{
				}

				void			clear()
				{
					if(_texture)				delete _texture;
					_texture=0;
					_refCount=1;
				}

				int				addRef()			{ return ++_refCount; }
				int				remRef()			{ return --_refCount; }
				int				getRefCount() const	{ return _refCount; }

				operator		ITexture*()	const	{ return _texture; }
		};

		typedef map<string,PTSItem>			TextureMap;
		TextureMap							_textures;

		IVideoDevice*						_device;

	public:

		ParticleTextureStore(IVideoDevice* dev)	:	_device(dev)			{}
		~ParticleTextureStore();

		ITexture*							getTexture(const string& texFile);
		void								releaseTexture(const string& texFile);
};

#endif